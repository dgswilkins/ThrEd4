# PES overview

The PES format is a hybrid embroidery design and embroidery command file
format for [Brother Industries](https://en.wikipedia.org/wiki/Brother_Industries) and 
[Bernina International](https://en.wikipedia.org/wiki/Bernina_International) series of
embroidery machines, among others.

PES files contain at their core two different parts for two completely
different purposes. The PES part contains the design information for
Brother PE-Design and perhaps similar applications. And the PEC part
contain the design name, colors, embroidery machine commands, and
graphics for the embroidery machine. Since the PEC block is the same in
all versions so that it's always backwards compatible with the hardware
of the Brother and Bernina embroidery machines, software made by others
therefore tends to only read that data and turns those commands into
line segments. However, later versions of PES version 4+ have addendum
information about the PEC block that wouldn't interfere with the
processing of the PEC block but contains information relevant to the
embroidery machine.

In PES version 1, the only blocks contained within the version 1 of the
format were the [CEmbOne](#cembone) and [CSewSeg](#csewseg) blocks. These
blocks contain the affine transformation data for the position data, and 
the position data. These can be grouped in a few different ways but they 
only ever contain sewing segment information. So within PESv1 we end up
with two copies of the same data in each of their respective blocks. The 
PES data has a copy of the segments and the PEC data has a copy of the
segments.

Later versions of PES have higher level objects which contain vector
shapes and information about how these are filled when it renders. If a
PES version 6 design, for example, contained a rectangle object, the PES
block would have a [CEmbRect](#cembrect) section. The PEC block would 
then have all the commands for the machine to sew that according to the 
pathing chosen by the instance that saved the PES file. The reason for 
this is if Brother PE-Design loses the information it used to generate the
PEC data namely that it was a rectangle at this specific location it becomes
impossible to do operations like change the spacing between the scanlines
within the rectangle, because after reloading the file all the program would
have access to are the line segments; losing the shape makes rerendering that
shape with a different fill density impossible.

Fully grasping these elements of the format allow for creating
interesting custom files or ways to modify them in logically consistent
but weird methods.

Some later PES versions also contain addendum PEC information. It occurs
in ways that cannot interfere with reading a PEC block standardly but
seems to provide additional expected information in PES version 4 and
above, and is expected on later machines.

## PES File Format

A PES format file consists of a version string followed by the PEC seek value
and two main sections, the [PES Section](#pes-objects) and the [PEC Section](#pec-section).
The PES section must immediately follow the seek value, so the version
string and the PEC seek value are often considered to be part of the PES
header. The PEC section can occur anywhere after that but the seek value must
point to it. Outside of directly manipulating the pointers, these two sections
normally occur immediately after each other.

The PES and PEC sections are distinct and disjoint and have their own
conventions, purposes, formats, read values, etc. The PEC section is
a 16 bit format, so you will only find short integers in the PEC block,
whereas the PES section is a 32 bit format and also contains 4-byte integers
and floating point numbers. The PES section has many versions and valid things
in one version may be invalid in the next. The header is also different for 
each version. The PEC section however, needs to be consistent and remain 
backwards compatible so all future embroidery machines can read any PES files.

### Basic type definitions

All integer and floating point numbers are stored in little-endian byte
order unless otherwise noted.

| Type   | Bytes | Description                                                                                          |
| ------ | ----- | -----------------------------------------------------------------------------------------------------|
| `char` | 1     | ASCII text (not `NUL` terminated)                                                                    |
| `u8`   | 1     | 8 bit unsigned integer                                                                               |
| `u16`  | 2     | 16 bit unsigned integer                                                                              |
| `s16`  | 2     | 16 bit signed integer in stitch coordinate space where `1` represents 0.1 mm                         |
| `u32`  | 4     | 32 bit unsigned integer                                                                              |
| `f32`  | 4     | 32 bit [floating point number](https://en.wikipedia.org/wiki/Single-precision_floating-point_format) |

## Common PES Objects

Some forms such as [`geometry'](#geometry) and the [block description](#block-description) are found 
within many different PES sections.

### Geometry

| Type      | Bytes | Value | Description                                   |
| --------- | ----- | ----- | --------------------------------------------- |
| `section` | 16    |       | [`extents`](#extents), the hitbox for the particular object |
| `section` | 24    |       | [`affine_transform`](#affine-transform)       |

### Extents

The distinction between these two copies of the same data is currently
unknown. However, modifying these cause the selection box within
PE-Design to go a bit weird.

| Type  | Bytes | Description     |
| ----- | ----- | --------------- |
| `s16` | 2     | Left            |
| `s16` | 2     | Top             |
| `s16` | 2     | Right           |
| `s16` | 2     | Bottom          |
| `s16` | 2     | Left Position   |
| `s16` | 2     | Top Position    |
| `s16` | 2     | Right Position  |
| `s16` | 2     | Bottom Position |

### Affine transform

An [affine transformation](https://en.wikipedia.org/wiki/Affine_transformation_affine_transformation)
is 9 floating point values, usually stored as 6 (because the other three
are always 0, 0, 1). It’s a matrix by which any series of affine
[geometric operations](https://www.youtube.com/watch?v=vQ60rFwh2ig_geometric_operations)
namely scaling, skewing, and transforming can be performed in a single
calculation for any number of points. We should usually just use the
identity matrix here.

| Type  | Bytes | Value             | Description                   |
| ----- | ----- | ----------------- | ----------------------------- |
| `f32` | 4     | `1.0f` (identity) | Row 1 column 1, scale X       |
| `f32` | 4     | `0.0f` (identity) | Row 2 column 1, skew Y        |
| `f32` | 4     | `0.0f` (identity) | Row 1 column 2, skew X        |
| `f32` | 4     | `1.0f` (identity) | Row 2 column 2, scale Y       |
| `f32` | 4     | `0.0f` (identity) | Row 1 column 3, translation X |
| `f32` | 4     | `0.0f` (identity) | Row 2 column 3, translation Y |

### Block Description

These are the preamble for most shape forms like CEmbRect and CEmbCirc
sections and effectively all objects which can be filled shapes with
stitches. (Much credit to Josh Varga for figuring this data out).

| Type           | Bytes | Value                       | Description                                                                  |
| -------------- | ----- | --------------------------- | ---------------------------------------------------------------------------- |
| `section`      | 40    | [Block Geometry](#geometry) |  Extents & Affine Transformation                                             |
| `s16`          | 2     |                             | Is Open Path?                                                                |
| `s16`          | 2     |                             | UseOutline                                                                   |
| `s16`          | 2     |                             | Outline Stitch Type, 0 = Zigzag, 1 = run Stitch                              |
| `s16`          | 2     |                             | outline_color_index                                                          |
| `s16`          | 2     |                             | Zigzag Width                                                                 |
| `s16`          | 2     |                             | Zigzag Density                                                               |
| `s16`          | 2     |                             | Outline Run Pitch                                                            |
| `s16`          | 2     |                             | Run Stitch Run Times                                                         |
| `s16`          | 2     |                             | Triple Stitch Run Pitch                                                      |
| `s16`          | 2     |                             | Use OutLine Under Sewing.                                                    |
| `s16`          | 2     |                             | Use Sharp Corners                                                            |
| `section`      | ?     | [Motif](#motif)             |                                                                              |
| `s16`          | 2     |                             | EStitchOrVStitch?                                                            |
| `s16`          | 2     |                             | EvStitchTallWidth                                                            |
| `s16`          | 2     |                             | EvStitchHeight                                                               |
| `s16`          | 2     |                             | EvStitchPointWidth                                                           |
| `s16`          | 2     |                             | EVStitchRunTimes                                                             |
| `s16`          | 2     |                             | EvStitchStrokeTimes                                                          |
| `s16`          | 2     |                             | EvStitchArrange                                                              |
| `s16`          | 2     |                             | Use Fill?                                                                    |
| `s16`          | 2     |                             | Fill Type                                                                    |
| `s16`          | 2     |                             | Region Density                                                               |
| `s16`          | 2     |                             | Use Gradation                                                                |
| `short_array`  | 2 × N |                             | GradationValues                                                              |
| `s16`          | 2     |                             | UseAdditionalColorInGradation                                                |
| `s16`          | 2     |                             | Gradation Second Color                                                       |
| `s16`          | 2     |                             | IsStitchDirectionVariable                                                    |
| `s16`          | 2     |                             | StitchDirection                                                              |
| `s16`          | 2     |                             | fill color, index of the fill color value.                                   |
| `s16`          | 2     |                             | BaseSewingStepPitch                                                          |
| `s16`          | 2     |                             | BaseSewingStepFrequency                                                      |
| `s16`          | 2     |                             | BaseSewingStitchType                                                         |
| `s16`          | 2     |                             | FillHalfStitch                                                               |
| `s16`          | 2     |                             | useFillAreaUnderSewing                                                       |
| `s16`          | 2     |                             | FillAreaUnderSewingType, 0 = one direction, 1 = both directions.             |
| `s16`          | 2     |                             | RunningStitchPath, (comment "half-stitch?")                                  |
| `s16`          | 2     | `20`                        | ProgrammableFillPatternIndex (comment "20")                                  |
| `s16`          | 2     |                             | ProgrammableFillWidth                                                        |
| `s16`          | 2     |                             | ProgrammableFillHeight                                                       |
| `s16`          | 2     |                             | ProgrammableFillRowOrColumn                                                  |
| `s16`          | 2     |                             | ProgrammableFillRowOffset                                                    |
| `s16`          | 2     |                             | ProgrammableFillColumnOffset                                                 |
| `s16`          | 2     |                             | ProgrammableFillDirection (comment: "WHERE IS PULL COMP")                    |
| `s16`          | 2     |                             | UseBaseSewing (comment: "Motif section")                                     |
| `s16`          | 2     |                             | Pull Compensation                                                            |
| `s16`          | 2     | `0`, false (default)        | UseMotifPattern1                                                             |
| `section`      | ?     | [Motif](#motif)             | Motif                                                                        |
| `s16`          | 2     | `0`, false (default)        | UseMotifPattern2                                                             |
| `section`      | ?     | [Motif](#motif)             | Motif                                                                        |
| `s16`          | 2     |                             | MotifRowOffset                                                               |
| `s16`          | 2     |                             | MotifStitchDirection                                                         |
| `s16`          | 2     |                             | MotifRunPitch                                                                |
| `s16`          | 2     |                             | pipingStitchDirection                                                        |
| `s16`          | 2     |                             | pipingStitchDensity                                                          |
| `s16`          | 2     |                             | pipingStitchRunPitch                                                         |
| `s16`          | 2     |                             | pipingStitchRunFrequency                                                     |
| `s16`          | 2     |                             | CrossStitchSizeMeasure, 0 for size, 1 for ct, (comment: CrossStitch section) |
| `s16`          | 2     | `25` (default)              | CrossStitchSizeInMM                                                          |
| `s16`          | 2     | `2` (default)               | CrossStitchTimes                                                             |
| `s16`          | 2     | `20` (default)              | RadialDensity                                                                |
| `s16`          | 2     | `20` (default)              | RadialRunPitch                                                               |
| `s16`          | 2     |                             | RadialCenterX (comment: 961)                                                 |
| `s16`          | 2     |                             | RadialCenterY (comment: 847)                                                 |
| `s16`          | 2     | `20` (default)              | ConcentricCircleDensity                                                      |
| `s16`          | 2     | `20` (default)              | ConcentricCircleRunPitch                                                     |
| `s16`          | 2     |                             | ConcentricCircleCenterX (comment: 961)                                       |
| `s16`          | 2     |                             | ConcentricCircleCenterY (comment: 847)                                       |
| `s16`          | 2     | `20` (default)              | SpiralDensity                                                                |
| `s16`          | 2     | `20` (default)              | SpiralRunPitch                                                               |
| `s16`          | 2     |                             | StipplingRunPitch                                                            |
| `s16`          | 2     |                             | StipplingSpacing                                                             |
| `s16`          | 2     |                             | OutlineEntryv1                                                               |
| `float`        | 4     |                             | OutlineEntry                                                                 |
| `s16`          | 2     |                             | OutlineExitv1                                                                |
| `float`        | 4     |                             | OutlineExit                                                                  |
| `s16`          | 2     |                             | RegionEntryv1                                                                |
| `float`        | 4     |                             | RegionEntry                                                                  |
| `s16`          | 2     |                             | RegionExit                                                                   |
| `float`        | 4     |                             | RegionExitv1                                                                 |

### Motif

| Type      | Bytes | Value                      | Description                |
| --------- | ----- | -------------------------- | -------------------------- |
| `s16`     | 2     |                            | Tile line count            |
| `section` |       | [`tile_lines`](#tile-line) |                            |

### Tile line

| Type          | Bytes   | Value | Description           |
| ------------- | ------- | ----- | --------------------- |
| `s16`         | 2       |       | Unknown               |
| `s16`         | 2       |       | Unknown               |
| `s16`         | 2       |       | Unknown               |
| `s16`         | 2       |       | Unknown               |
| `s16`         | 2       | _N_   | Tile line point count |
| `short_array` | 2 × _N_ |       | Point array           |

## PES Header

### Common Header

The first 12 bytes of the header section are common to all versions. PES
versions 1 through 4 have a fixed predefined [PEC thread palette](#pec-thread-palette)
of 64 threads. PES versions 5 and 6 replace this with configurable
threads.

| Type   | Bytes | Value    | Description    |
| ------ | ----- | -------- | -------------- |
| `char` | 4     | `"#PES"` | Identification |
| `char` | 4     | `"0001"` \| `"0020"` \| `"0025"` \| `"0030"` \| `"0040"` \| `"0050"` \|`"0055"` \| `"0056"` \| `"0060"` | Version 1, 2, 2.5, 3, 4, 5, 5.5, 5.6 or 6
| `u32`  | 4     |          | Absolute [`PEC_section`](#pec-section) byte offset |

### Version 1 header section

| Type   | Bytes | Value      | Description                                          |
| ------ | ----- | ---------- | ---------------------------------------------------- |
| `char` | 4     | `"#PES"`   | Identification                                       |
| `char` | 4     | `"0001"`   | Version 1                                            |
| `u32`  | 4     |            | Absolute [`PEC_section`](#pec-section) byte offset   |
| `u16`  | 2     | `0` \| `1` | Hoop size where `0` is 100×100 mm², else 130×180 mm² |
| `u16`  | 2     | `0` \| `1` | Use Existing Design Area                             |
| `u16`  | 2     |            | [segment block count](#csewseg) The number of objects. In version 1 this is the same as the number of CSewSeg blocks, as distinct from the number of segments within that block. In PE-Design objects are the things you can select. |

### Version 2 header section

| Type      | Bytes | Value                                | Description                                        |
| --------- | ----- | ------------------------------------ | -------------------------------------------------- |
| `char`    | 4     | `"#PES"`                             | Identification                                     |
| `char`    | 4     | `"0020"`                             | Version 2                                          |
| `u32`     | 4     |                                      | Absolute [`PEC_section`](#pec-section) byte offset |
| `section` | 4     | [`hoop_size`](#hoop-size-subsection) | Hoop size in mm                                    |
| `u16`     | 2     |                                      | Hoop Rotation, 90° 1 = rotate.                     |
| `u16`     | 2     |                                      | Unknown, design page background color?             |
| `u16`     | 2     |                                      | Unknown, design page foreground color?             |
| `u16`     | 2     |                                      | Unknown, show grid?                                |
| `u16`     | 2     |                                      | Unknown, with axes?                                |
| `u16`     | 2     |                                      | Unknown, snap to grid?                             |
| `u16`     | 2     |                                      | Unknown, grid interval?                            |
| `u16`     | 2     |                                      | Unknown, P9 curves?                                |
| `u16`     | 2     |                                      | Unknown, optimize entry-exit points?               |

### Version 2.5 header section

| Type      | Bytes | Value                                | Description                                        |
| --------- | ----- | ------------------------------------ | -------------------------------------------------- |
| `char`    | 4     | `"#PES"`                             | Identification                                     |
| `char`    | 4     | `"0025"`                             | Version 2.5                                        |
| `u32`     | 4     |                                      | Absolute [`PEC_section`](#pec-section) byte offset |
| `u16`     | 2     | `1` (typical)                        | Unknown                                            |
| `u16`     | 2     | `0` (typical)                        | Unknown                                            |
| `section` | 4     | [`hoop_size`](#hoop-size-subsection) | Hoop size in mm                                    |
| `u16`     | 2     |                                      | Hoop Rotation, 90° 1 = rotate.                     |
| `u16`     | 2     |                                      | Unknown, design page background color?             |
| `u16`     | 2     |                                      | Unknown, design page foreground color?             |
| `u16`     | 2     |                                      | Unknown, show grid?                                |
| `u16`     | 2     |                                      | Unknown, with axes?                                |
| `u16`     | 2     |                                      | Unknown, snap to grid?                             |
| `u16`     | 2     |                                      | Unknown, grid interval?                            |
| `u16`     | 2     |                                      | Unknown, P9 curves?                                |
| `u16`     | 2     |                                      | Unknown, optimize entry-exit points?               |
| `u16`     | 2     | `1` (typical)                        | The number of Objects, this is the number of objects within the PES document and is not limited to the number of CSewSeg blocks. If there are two SewSeg objects and 2 Rectangle objects, this will be 4. |

### Version 3 header section

| Type      | Bytes | Value                                | Description                                        |
| --------- | ----- | ------------------------------------ | -------------------------------------------------- |
| `char`    | 4     | `"#PES"`                             | Identification                                     |
| `char`    | 4     | `"0030"`                             | Version 3                                          |
| `u32`     | 4     |                                      | Absolute [`PEC_section`](#pec-section) byte offset |
| `u16`     | 2     | `1` (typical)                        | Unknown                                            |
| `char`    | 2     | `01` \| `02` \| `10`                 | Subversion?                                        |
| `section` | 4     | [`hoop_size`](#hoop-size-subsection) | Hoop size in mm                                    |
| `u16`     | 2     |                                      | Hoop Rotation, 90° 1 = rotate.                     |
| `u16`     | 2     |                                      | Unknown, design page background color?             |
| `u16`     | 2     |                                      | Unknown, design page foreground color?             |
| `u16`     | 2     |                                      | Unknown, show grid?                                |
| `u16`     | 2     |                                      | Unknown, with axes?                                |
| `u16`     | 2     |                                      | Unknown, snap to grid?                             |
| `u16`     | 2     |                                      | Unknown, grid interval?                            |
| `u16`     | 2     |                                      | Unknown, P9 curves?                                |
| `u16`     | 2     |                                      | Unknown, optimize entry-exit points?               |

### Version 4 header section

| Type      | Bytes | Value                                                    | Description                                        |
| --------- | ----- | -------------------------------------------------------- | -------------------------------------------------- |
| `char`    | 4     | `"#PES"`                                                 | Identification                                     |
| `char`    | 4     | `"0040"`                                                 | Version 4                                          |
| `u32`     | 4     |                                                          | Absolute [`PEC_section`](#pec-section) byte offset |
| `u16`     | 2     | `1` (typical)                                            | Unknown                                            |
| `char`    | 2     | `01` \| `02` \| `10`                                     | Subversion?                                        |
| `section` |       | [`description_strings`](#description-strings-subsection) |                                                    |
| `u8`      | 2     | `0` (typical)                                            | Unknown                                            |
| `section` | 4     | [`hoop_size`](#hoop-size-subsection)                     | Hoop size in mm                                    |
| `u16`     | 2     |                                                          | Hoop Rotation, 90° 1 = rotate.                     |
| `u16`     | 2     | `7` (typical)                                            | Unknown, design page background color?             |
| `u16`     | 2     | `19` (typical)                                           | Unknown, design page foreground color?             |
| `u16`     | 2     | `0` \| `1` \| ?                                          | Unknown, show grid?                                |
| `u16`     | 2     | `1` (typical)                                            | Unknown, with axes?                                |
| `u16`     | 2     | `0` \| `1` \| ?                                          | Unknown, snap to grid?                             |
| `u16`     | 2     | `100` (typical)                                          | Unknown, grid interval?                            |
| `u8`      | 1     | `1` (typical)                                            | Unknown                                            |
| `u8`      | 7     | `0` (typical)                                            | Unknown                                            |
| `u16`     | 2     | `1` (typical)                                            | Unknown                                            |

### Version 5 header section

| Type      | Bytes    | Value                                                    | Description                                        |
| --------- | -------- | -------------------------------------------------------- | -------------------------------------------------- |
| `char`    | 4        | `"#PES"`                                                 | Identification                                     |
| `char`    | 4        | `"0050"`                                                 | Version 5                                          |
| `u32`     | 4        |                                                          | Absolute [`PEC_section`](#pec-section) byte offset |
| `u16`     | 2        | `1` (typical)                                            | "Hoop Size Indicator"                              |
| `char`    | 2        | `01` \| `02` \| `10`                                     | Subversion?                                        |
| `section` |          | [`description_strings`](#description-strings-subsection) |                                                    |
| `u16`     | 2        | `0` (typical)                                            | "Optimize Hoop Change"                             |
| `section` | 4        | [`hoop_size`](#hoop-size-subsection)                     | Hoop size in mm                                    |
| `u16`     | 2        |                                                          | Hoop Rotation, 90° 1 = rotate.                     |
| `u16`     | 2        | `7` (typical)                                            | Design Page Background Color?                      |
| `u16`     | 2        | `19` (typical)                                           | Design Page Foreground Color                       |
| `u16`     | 2        | `0` \| `1` \| ?                                          | "Show Grid"                                        |
| `u16`     | 2        | `1` (typical)                                            | "With Axes"                                        |
| `u16`     | 2        | `0` \| `1` \| ?                                          | "Snap To Grid"                                     |
| `u16`     | 2        | `100` (typical)                                          | "Grid Interval"                                    |
| `u16`     | 2        | `1` (typical)                                            | Unknown                                            |
| `u16`     | 2        | `0` (typical)                                            | "OptimizeEntryExitPoints"                          |
| `u8`      | 1        | _S0_ `0` (typical)                                       | "fromImageStringLength"                            |
| `char`    | _S0_     |                                                          | Path string                                        |
| `section` | 24       | [`affine_transform`](#affine-transform)                  |                                                    |
| `u16`     | 2        | _S1_ `0` (typical)                                       | Number of programmable fill patterns               |
| `section` | _S1_ * ? | [`fill_patterns`](#programmable-fill-subsection)         |                                                    |
| `u16`     | 2        | _S2_ `0` (typical)                                       | Number of programmable Motif                       |
| `section` | _S2_ * ? | [`motif`](#motif)                                        |                                                    |
| `u16`     | 2        | _S3_ `0` (typical)                                       | Number of feather patterns                         |
| `section` | _S3_ * ? | [`feather_patterns`](#feather-pattern-subsection)        |                                                    |
| `section` |          | [`color_list`](#color-list-subsection)                   |                                                    |
| `u16`     | 2        | `1` (typical)                                            | The number of Objects, this is the number of objects within the PES document and is not limited to the number of CSewSeg blocks. If there are two SewSeg objects and 2 Rectangle objects, this will be 4. |

### Version 6 header section

| Type      | Bytes    | Value                                                    | Description                                         |
| --------- | -------- | -------------------------------------------------------- | --------------------------------------------------- |
| `char`    | 4        | `"#PES"`                                                 | PES Identification                                  |
| `char`    | 4        | `"0060"`                                                 | Version 6                                           |
| `u32`     | 4        |                                                          | Absolute [`PEC_section`](#pec-section) byte offset  |
| `u16`     | 2        | `1` (typical)                                            | Hoop Size Indicator                                 |
| `char`    | 2        | `01` \| `02` \| `10`                                     | Subversion?                                         |
| `section` |          | [`description_strings`](#description-strings-subsection) |                                                     |
| `u16`     | 2        | `0` (typical)                                            | Optimize Hoop Change                                |
| `u16`     | 2        | `0` (typical)                                            | Design Page Is Custom                               |
| `section` | 4        | [`hoop_size`](#hoop-size-subsection)                     | Hoop size in mm                                     |
| `u16`     | 2        |                                                          | Hoop Rotation, 90° 1 = rotate.                      |
| `u16`     | 2        | `200` (typical)                                          | DesignWidth                                         |
| `u16`     | 2        | `200` (typical)                                          | DesignHeight                                        |
| `u16`     | 2        | `100` (typical)                                          | DesignPageWidth                                     |
| `u16`     | 2        | `100` (typical)                                          | DesignPageHeight                                    |
| `u16`     | 2        | `100` (typical)                                          | Undisplayed Unknown Design Property, must fall within same range as designPageWidth and Height. Makes no difference other than validation check.|
| `u16`     | 2        | `7` (typical)                                            | "Design Page Background Color"                      |
| `u16`     | 2        | `19` (typical)                                           | "Design Page Foreground Color"                      |
| `u16`     | 2        | `0` \| `1` \| ?                                          | "Show Grid"                                         |
| `u16`     | 2        | `1` (typical)                                            | "With Axes"                                         |
| `u16`     | 2        | `0` (typical)                                            | "Snap To Grid"                                      |
| `u16`     | 2        | `15` \| `100` \| ?                                       | "Grid Interval"                                     |
| `u16`     | 2        | `1`                                                      | Unknown                                             |
| `u16`     | 2        | `0` (typical)                                            | "OptimizeEntryExitPoints"                           |
| `u8`      | 1        | _S0_ `0` (typical)                                       | "fromImageStringLength"                             |
| `char`    | _S0_     |                                                          | Path string                                         |
| `section` | 24       | [`affine_transform`](#affine-transform)                  |                                                     |
| `u16`     | 2        | _S1_ `0` (typical)                                       | Number of programmable fill patterns                |
| `section` | _S1_ * ? | [`fill_patterns`](#programmable-fill-subsection)         |                                                     |
| `u16`     | 2        | _S2_ `0` (typical)                                       | Number of programmable Motif                        |
| `section` | _S2_ * ? | [`motif`](#motif)                                        |                                                     |
| `u16`     | 2        | _S3_ `0` (typical)                                       | Number of feather patterns                          |
| `section` | _S3_ * ? | [`feather_patterns`](#feather-pattern-subsection)        |                                                     |
| `section` |          | [`color_list`](#color-list-subsection)                   |                                                     |
| `u16`     | 2        | `1` (typical)                                            | The number of Objects, this is the number of objects within the PES document and is not limited to the number of CSewSeg blocks. If there are two SewSeg objects and 2 Rectangle objects, this will be 4. |

### Programmable fill subsection

### Motif subsection

### Feather Pattern Subsection

### Hoop size subsection

| Type  | Bytes | Value | Description        |
| ----- | ----- | ----- | ------------------ |
| `u16` | 2     |       | Hoop width \[mm\]  |
| `u16` | 2     |       | Hoop height \[mm\] |

### Description strings subsection

| Type   | Bytes  | Value  | Description                |
| ------ | ------ | ------ | -------------------------- |
| `u8`   | 1      | _S1_   | Length of following string |
| `char` | _S1_   |        | Design string              |
| `u8`   | 1      | _S2_   | Length of following string |
| `char` | _S2_   |        | Category string            |
| `u8`   | 1      | _S3_   | Length of following string |
| `char` | _S3_   |        | Author string              |
| `u8`   | 1      | _S4_   | Length of following string |
| `char` | _S4_   |        | Keywords string            |
| `u8`   | 1      | _S5_   | Length of following string |
| `char` | _S5_   |        | Comments string            |

### Color list subsection

| Type      | Bytes | Value                        | Description                           |
| --------- | ----- | ---------------------------- | ------------------------------------- |
| `u16`     | 2     |                              | Number of following color subsections |
| `section` |       | [`color`](#color-subsection) |                                       |

### Color subsection

| Type   | Bytes  | Value                                     | Description                                                         |
| ------ | ------ | ----------------------------------------- | ------------------------------------------------------------------- |
| `u8`   | 1      | _S1_                                      | Length of following string                                          |
| `char` | _S1_   | `"336"` \| `"575"` \| etc.                | Color code, numeric 0-6 digit number.                               |
| `u8`   | 1      | `0 ` – `255`                              | Red component of RGB color model                                    |
| `u8`   | 1      | `0 ` – `255`                              | Green component of RGB color model                                  |
| `u8`   | 1      | `0 ` – `255`                              | Blue component of RGB color model                                   |
| `u8`   | 1      | `0` (typical)                             | Unknown (likely padding, writes color as 32 bit rather than 24 bit) |
| `u32`  | 4      | `0x0` \| `0x1` \| `0x2` \| `0xA` \| `0xB` | Color Type. 0 is custom list color. 1 is list color (Eg. 1615 MA Polyneon). 2 is an applique, description is either 100, 101, 102 (APPLIQUE MATERIAL, APPLIQUE POSITION, APPLIQUE respectively). A is custom color, not from a list eg (Red, Wilcom). B is Version 4 special list brother specific color. |
| `u8`   | 1      | _S2_                                      | Length of following string                                          |
| `char` | _S2_   | `"Magenta"` \| `"Orange"` \| etc.         | Description.                                                        |
| `u8`   | 1      | _S3_                                      | Length of following string                                          |
| `char` | _S3_   | `"ISACORD"` \| `"RA Polyester"` \| etc.   | Brand                                                               |
| `u8`   | 1      | _S4_                                      | Length of following string                                          |
| `char` | _S4_   | `"My thread Chart"` \| `"Stock"` \| etc.  | Color chart name (Usually _S4_ is 0, and this is absent)            |

## CEmbOne

The CEmbOne section is always 73 bytes in size. The data within the
object is almost identical to the data for the appended headers of additional
[CSewSeg](#PES-CSewSeg-section "wikilink") objects. The data here is the
header information for the first (and often only) group within that
section.

| Type      | Bytes | Value                                          | Description                |
| --------- | ----- | ---------------------------------------------- | -------------------------- |
| `u16`     | 2     | `7`                                            | Length of following string |
| `char`    | 7     | `"CEmbOne"`                                    | CEmbOne identification     |
| `section` | 60    | [CSewSeg_comon_header](#csewseg-common-header) |                            |

## CSewSeg

The first [CSewSeg segment block](#csewseg-segment-block)'s header is the 
data of the [CEmbOne](#cembone) block. If the [PES header](#pes-header)
declares the number of [csewseg segment blocks](#csewseg-segment-block) 
to be 1, then this block contains only the 
[CSewSeg segment block](#csewseg-segment-block). If zero then this block
would have been omitted. But, if greater than one, then additional
[headers](#cembone-header) and 
[CSewSeg segment block](#csewseg-segment-block) are appended within this
block.

| Type      | Bytes | Value       | Description                                                |
| --------- | ----- | ----------- | ---------------------------------------------------------- |
| `u16`     | 2     | `7`         | Length of following string                                 |
| `char`    | 7     | `"CSewSeg"` | CSewSeg identification                                     |
| `section` |       |             | [CSewSeg_segment_block](#csewseg-segment-block)            |
| `section` |       |             | [CSewSeg_header](#csewseg-header) (optional)               |
| `section` |       |             | [CSewSeg_segment_block](#csewseg-segment-block) (optional) |
| ...       |       |             | Additional header / segment-block sequences (optional)     |

### CSewSeg segment block

| Type      | Bytes | Value                                                    | Description                    |
| --------- | ----- | -------------------------------------------------------- | ------------------------------ |
| `section` |       | [`csewseg_stitch_list`](#csewseg-stitch-list-subsection) | CSewSeg stitch list subsection |
| `section` |       | [`csewseg_color_list`](#csewseg-color-list-subsection)   | CSewSeg color list subsection  |

#### CSewSeg stitch list subsection

The CSewSeg stitch list subsection is divided into stitch blocks. The
last block does not end with the continuation code `0x8003` and so is
2 bytes shorter than the others. Some files seem to limit the number of
stitches to a maximum of 1000 per block, is this required?

| Type      | Bytes   | Value                                                 | Description                                                             |
| --------- | ------- | ----------------------------------------------------- | ----------------------------------------------------------------------- |
| `u16`     | 2       | `0` \| `1` \| ?                                       | Stitch type where `0` means a normal stitch and `1` means a jump stitch |
| `u16`     | 2       |                                                       | Thread index for block + 1                                              |
| `u16`     | 2       | _N_                                                   | Number of following coordinates                                         |
| `section` | 4 × _N_ | [CSewSeg_coordinates](#csewseg-coordinate-subsection) |                                                                         |
| `u16`     | 2       | `0x8003` \| ?                                         | Continuation code where `0x8003` means list continues with another following block, with the last block not having this field at all |

#### CSewSeg coordinate subsection

| Type  | Bytes | Value | Description         |
| ----- | ----- | ----- | ------------------- |
| `s16` | 2     |       | Stitch x coordinate |
| `s16` | 2     |       | Stitch y coordinate |

#### CSewSeg color list subsection

| Type      | Bytes   | Value                                        | Description                                  |
| --------- | ------- | -------------------------------------------- | -------------------------------------------- |
| `u16`     | 2       | _N_                                          | Number of following colors                   |
| `section` | 4 × _N_ | [`csewseg_color`](#csewseg-color-subsection) |                                              |

#### CSewSeg color subsection

| Type  | Bytes | Value | Description                                                                                                                                                                                                                                               |
| ----- | ----- | ----- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `u16` | 2     |       | Block index where change of thread takes effect, starting from zero                                                                                                                                                                                       |
| `u16` | 2     |       | In PES versions 1 through 4 this is a [PEC thread palette](#pec-thread-palette) index and in versions 5 and 6 this is a [thread index](#color-list-subsection "wikilink") defined in the [PES header section](#Header "wikilink") |

### CsewSeg Header
| Type      | Bytes | Value                                          | Description                                    |
| --------- | ----- | ---------------------------------------------- | ---------------------------------------------- |
| `u16`     | 2     | `7`                                            | Length of following string                     |
| `char`    | 7     | `"CSewSeg"`                                    | CSewSeg identification                         |
| `section` | 60    | [CSewSeg_comon_header](#csewseg-common-header) |                                                |

### CSewSeg Common Header

| Type      | Bytes | Value                       | Description                       |
| --------- | ----- | --------------------------- | --------------------------------- |
| `section` | 40    | [Block Geometry](#geometry) | Extents and Affine Transformation |
| `u16`     | 2     | `1` (typical)               | Unknown                           |
| `s16`     | 2     |                             | x coordinate translation?         |
| `s16`     | 2     |                             | y coordinate translation?         |
| `s16`     | 2     |                             | width                             |
| `s16`     | 2     |                             | height                            |
| `u8`      | 8     | `0` (typical)               | 8 bytes of unknown, zero.         |
| `u16`     | 2     |                             | CSewSeg block count               |

## CEmbCirc

| Type      | Bytes | Value                                     | Description                |
| --------- | ----- | ----------------------------------------- | -------------------------- |
| `u16`     | 2     | `8`                                       | Length of following string |
| `char`    | 8     | `"CEmbCirc"`                              | CEmbCirc identification    |
| `section` | ?     | [`block_description`](#block-description) |                            |
| `u16`     | 2     |                                           | unknown                    |
| `u16`     | 2     |                                           | Type                       |
| `u16`     | 2     |                                           | start angle                |
| `u16`     | 2     |                                           | end angle                  |
| `u16`     | 2     |                                           | left                       |
| `u16`     | 2     |                                           | top                        |
| `u16`     | 2     |                                           | right                      |
| `u16`     | 2     |                                           | bottom                     |

## CEmbRect

| Type      | Bytes        | Value                                     | Description                |
| --------- | ------------ | ----------------------------------------- | -------------------------- |
| `u16`     | 2            | `8`                                       | Length of following string |
| `char`    | 8            | `"CEmbRect"`                              | CEmbRect identification    |
| `section` | 240 + motifs | [`block_description`](#block-description) |                            |
| `u16`     | 2            |                                           | corner_radius              |
| `u16`     | 2            |                                           | unknown                    |
| `u16`     | 2            |                                           | left                       |
| `u16`     | 2            |                                           | top                        |
| `u16`     | 2            |                                           | right                      |
| `u16`     | 2            |                                           | bottom                     |

## CEmbLine

| Type      | Bytes      | Value                                     | Description                                 |
| --------- | ---------- | ----------------------------------------- | ------------------------------------------- |
| `u16`     | 2          | `8`                                       | Length of following string                  |
| `char`    | 8          | `"CEmbLine"`                              | CEmbLine identification                     |
| `section` | ?          | [`block_description`](#block-description) |                                             |
| `u16`     | 2          |                                           | unknown                                     |
| `u16`     | 2          |                                           | vertex count, number of vertexes to expect. |
| `section` | count \* 8 |                                           | series of vertexes.                         |

### Point List

| Type  | Bytes | Value | Description |
| ----- | ----- | ----- | ----------- |
| `u16` | 2     |       | p1          |
| `u16` | 2     |       | p2          |
| `u16` | 2     |       | p3          |
| `u16` | 2     |       | type        |

## CEmbPunch

| Type   | Bytes | Value         | Description                |
| -------| ----- | ------------- | -------------------------- |
| `u16`  | 2     | `9`           | Length of following string |
| `char` | 9     | `"CEmbPunch"` | CEmbPunch identification   |
| ?      | ?     |               | Unknown                    |

## CSewFigSeg

| Type   | Bytes | Value          | Description                |
| ------ | ----- | -------------- | -------------------------- |
| `u16`  | 2     | `10`           | Length of following string |
| `char` | 10    | `"CSewFigSeg"` | CSewFigSeg identification  |
| ?      | ?     |                | Unknown                    |

## CLetter

| Type   | Bytes | Value       | Description                |
| ------ | ----- | ----------- | -------------------------- |
| `u16`  | 2     | `7`         | Length of following string |
| `char` | 7     | `"CLetter"` | CLetter identification     |
| ?      | ?     |             | Unknown                    |

## CEmbNText

| Type   | Bytes | Value              | Description                |
| ------ | ----- | ------------------ | -------------------------- |
| `u16`  | 2     | `9`                | Length of following string |
| `char` | 9     | `"CEmbNText"`      | CEmbNText identification   |
| ?      | ?     |                    | Unknown                    |
| `u16`  | 2     | `0xFFFF` (typical) | Unknown                    |
| `u16`  | 2     | `0` (typical)      | Unknown                    |

## PEC Addendum

There is a block in that exists in V4 to V6 and likely beyond of PES,
which includes what appears to be addendum PEC information and occurs
after the traditional PEC graphics block ends.

Without this block version 6 of the files cannot be detected on the
various embroidery machines. For a successful write of V6 it must be
included.

### PEC Addendum header

The block starts with the count index, where 0 is FF and 1 is 00. In the
same N-1 format of the PEC colors. This is followed by the PEC colors.
Though in Brother's own rendering of these files using 0 is fine for the
colors as they also use in the PEC blocks. One assumes since they
included the data in the PES header it will know what the colors
actually are.

This is then padded out by 0x20 just as the PEC header is, to 128 bytes.

A blank version of this will be 0xFF followed by 127 bytes of 0x20.

### PEC Addendum color info

For each color in the Addendum header, we have 0x90 bytes (144 bytes) of
information. The only thing that seems to dictate the data that is used
here is the color used. Each color seems to result in a different set of
data. However, brother has some colors produce an entirely 0 amount of
data. The only thing that matters for this data is what color was used.

### PEC Addendum RGB List

After the blocks of 144 bytes for each color, we produce a simple list
of the RGB colors of these threads. So if we had 5 colors we produce 15
bytes of RGBRGBRGBRGBRGB dumped after the last color info block.

In Version 6, we add two bytes of 0. 00 00 after the RGB list. This
isn't included in V5 or V4. It is unknown if it's part of the block or
something related to this being the end of file or something.

## Write PES V.1

We can read PES and PEC for all versions by reading the color data and
stitch data from the PEC block. But, the typical use case is going to be
writing a successful PES file. \[all writes should be done Little
Endian\].

We must write the following things to disk.

  - 8 bytes: "\#PES0001"
  - 4 bytes: Location of PEC block.
  - 2 bytes: 1 (scale to fit)
  - 2 bytes: 1 (hoop size, 130mm 180mm)
  - 2 bytes: 1 (we are going to write only one stitchblock object).
  - 4 bytes: FF FF 00 00, end the header.

### Write CEmbOne

  - 2 bytes: 7 (length of text "EmbOne")
  - 7 bytes: "EmbOne"
  - 2 bytes: 0, minX (we're passing on this)
  - 2 bytes: 0, minY (we're passing on this)
  - 2 bytes: 0, maxX (we're passing on this)
  - 2 bytes: 0, maxY (we're passing on this).
  - 2 bytes: 0, minX (we're passing on this)
  - 2 bytes: 0, minY (we're passing on this)
  - 2 bytes: 0, maxX (we're passing on this)
  - 2 bytes: 0, maxY (we're passing on this).
  - 4 bytes: float 1, 00 00 80 3F (Scale X)
  - 4 bytes: float 0, 00 00 00 00 (Skew_X)
  - 4 bytes: float 0, 00 00 00 00 (Skew_Y)
  - 4 bytes: float 1, 00 00 80 3F (Scale_Y)
  - 4 bytes: float (350 + HoopWidth/2 - WidthOfDesign/2), 350 is the
    distance from 0,0 that the 130mm x 180mm hoop is stored, if we add
    half the HoopWidth (1300 / 2) and subtract half our design's width.
    The embroidery will be centered in the hoop.
  - 4 bytes float (100 + HoopHeight/2 - HeightOfDesign/2), 100 is the
    distance from 0,0 that our 130mm x 180mm hoop is stored, if we add
    half the HoopHeight (1800 / 2) and subtract half our design's
    height. The embroidery will be centered in the hoop.
  - 2 bytes: 1, unknown.
  - 2 bytes: 0, X-location (we're passing on this).
  - 2 bytes 0, Y-location (we're passing on this).
  - 2 bytes width, WidthOfDesign
  - 2 bytes height, WidthOfDesign.
  - 4 bytes, 0, unknown.
  - 4 bytes, 0 unknown.
  - 2 bytes, number of segments + (2 \* colorChanges), this is going to
    be the number of segments we will need to use.

<!-- end list -->

  - 4 bytes FF FF 00 00, end block.

### Write CSewSeg

  - 2 bytes: 7, length of "CSewSeg"
  - 7 bytes: "CSewSeg"

### Writing Segment Information

For these, we must ensure our segments are flagged 0,1,0,1,0,1,0. That
our jumps are 0, that our stitches are 1, that our color changes are 0,
we pad with jumps and stitches to nowhere to maintain this. We assume
that we jump to the location, then color change, then stitch as
relevant.

#### Color Change, Initialize

  - 2 bytes: 1 this is a stitch.
  - 2 bytes color, color index.
  - 2 bytes 2, needs a length of at least two to not be a color change.
  - 2 byes: X, current position X.
  - 2 bytes: Y, current position Y.
  - 2 byes: X, current position X.
  - 2 bytes: Y, current position Y.
  - 2 bytes 0x8001, 01 80. End Segment
  - 2 bytes: 0, non-stitching.
  - 2 bytes, color, color index.
  - 2 bytes, 2, needs a length of at least two to not be a color change.
  - 2 byes: X, current position X.
  - 2 bytes: Y, current position Y.
  - 2 byes: X, current position X.
  - 2 bytes: Y, current position Y.
  - 2 bytes 0x8001, 01 80. End Segment

(We didn't call a color change event, but we ensured that the initial
color is established)

#### Color Change, Middle

  - 2 bytes: 0, we are not sewing this
  - 2 bytes: Color, color index. Index of old color.
  - 2 byte: 1, length of stitches.
  - 2 byes: X, current position X.
  - 2 bytes: Y, current position Y.
  - 2 bytes 0x8001, 01 80. End Segment
  - 2 bytes: 1, we are sewing this.
  - 2 bytes: Color, color index. Index of new color.
  - 2 bytes: 2, length two.
  - 2 byes: X, current position X.
  - 2 bytes: Y, current position Y.
  - 2 byes: X, current position X.
  - 2 bytes: Y, current position Y.
  - 2 bytes 0x8001, 01 80. End Segment

(We invoked a color change, and then a sewable event that went nowhere,
to make sure the color is established.)

#### Jump

  - 2 bytes, 0, we are not stitching.
  - 2 bytes, Color, whatever color we're on.
  - 2 bytes, length, the number of stitches in this jump, including
    start and end (minimum 2).
  - NN bytes, X position, Y position (relative to the position given to
    the StitchGroup description).
  - 2 bytes 0x8001, 01 80. End Segment

#### Stitch

  - 2 bytes, 1, we are stitching.
  - 2 bytes, Color, whatever color we're on.
  - 2 bytes, length, the number of stitches in this block, including
    start and end (minimum 2).
  - NN bytes, X position, Y position (relative to the position given to
    the StitchGroup description).
  - 2 bytes 0x8001, 01 80. End Segment

#### Color change

We now write the color change log.

  - 2 bytes: Number of color changes

For each color change:

  - 2 bytes: Section at which this change occurred. (Note this must
    account for the initialization and the padding sections we added.
  - 2 bytes: Color Change, old index value.

Continue to [Write PEC](Embroidery_format_PEC#Writing_PEC "wikilink")

## Write PES v.6

When writing PES v.6 we must note that the PES colors are not the index
colors from the PEC-Threads but rather the colors we defined in the
header.

  - Write, 8 bytes, "\#PES0060"
  - Write, 2 bytes, 1, // 0 = 100x100 else 130x180 or above
  - Write, 1 byte, 0x30, dunno
  - Write, 1 byte, 0x32, dunno.
  - Write, 1 byte, name length (if this is 0, name is omitted)
  - Write, name length bytes, name.
  - Write, 1 byte, category length (if this is 0, category is omitted)
  - Write, category length bytes, category.
  - Write, 1 byte, author length (if this is 0, author is omitted)
  - Write, author length bytes, author.
  - Write, 1 byte, keywords length (if this is 0, keywords is omitted)
  - Write, keywords length bytes, keywords.
  - Write, 1 byte, comments length (if this is 0, comment is omitted)
  - Write, comment length bytes, comment.
  - Write, 2 bytes, 0, optimizeHoopChange false
  - Write, 2 bytes, 0, DesignPageIsCustom false.
  - Write, 2 bytes, 0x64, HoopWidth
  - Write, 2 bytes, 0x64, HoopHeight
  - Write, 2 bytes, 0, Design Page Area
  - Write, 2 bytes, 0xC8, designWidth
  - Write, 2 bytes, 0xC8, designHeight
  - Write, 2 bytes, 0x64, designPageSectionWidth
  - Write, 2 bytes, 0x64, designPageSectionHeight
  - Write, 2 bytes, 0x64, p6, unknown but 100.
  - Write, 2 bytes, 0x07, designPageBackgroundColor
  - Write, 2 bytes, 0x13, designPageForegroundColor
  - Write, 2 bytes, 0x01, ShowGrid true.
  - Write, 2 bytes, 0x01, WithAxes true
  - Write, 2 bytes, 0x00, SnapToGrid false
  - Write, 2 bytes 100, GridInterval 100
  - Write, 2 bytes, 0x01, p9, unknown
  - Write, 2 bytes, 0, OptimizeEntryExitPoints false
  - Write, 1 byte, 0, fromImageStringLength, if non-zero file name must
    follow.
  - Write, 4 bytes, float(1f), image scaleX
  - Write, 4 bytes, float(0f), image
  - Write, 4 bytes, float(0f), image
  - Write, 4 bytes, float(1f), image scaleY
  - Write, 4 bytes, float(0f), image
  - Write, 4 bytes, float(0f), image
  - Write, 2 bytes, 0, numberOfProgrammableFillPatterns
  - Write, 2 bytes, 0, NumberOfMotifPatterns
  - Write, 2 bytes, 0, NumberOfFeatherPatterns
  - Write, 2 bytes, \#colors, numberOfThreads.

For each thread:

  - Write, 1 byte, catalogNumber string length
  - Write, 0-6 byte, catalogNumber should be 0-6 digits long numeric.
  - Write, 1 byte, red
  - Write, 1 byte, green
  - Write, 1 byte, blue
  - Write, 1 bytes, 0, unknown.
  - Write, 4 bytes, 0xA, thread type.
  - Write, 1 byte, Description length
  - Write, n bytes, Description
  - Write, 1 byte, Brand length
  - Write, n bytes, Brand
  - Write, 1 byte, Chart length
  - Write, n bytes, Chart

After threads, back to header.

  - Write, 2 bytes, 1, Objects In Pes.

Write block data in the same way as V.1, except using predefined colors
rather than PEC Thread.

  - Write CEmbOne block
  - Write CSewSeg block

We should end after the color log of the CSewSeg block. With the four
byte, 0x0000 0x0000 terminator.

### Write Order Data

  - Write, 8 bytes, 0 0 0 0 0 0 0 0, (this makes 12 zeros in a row with
    the terminator).
  - Write, n order data chunks. Where n is the same as the number of
    color changes we wrote in the colors section.

Each Order Chunk.

  - Write, 4 bytes, i
  - Write, 4 bytes, 0

So for 3 elements we would write (not including the 00 00 00 00
terminator this comes right after): 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 01 00 00 00 00 00 00 00 02 00 00 00 00 00 00 00

Continue to [Write PEC](Embroidery_format_PEC#Writing_PEC "wikilink")

### Write Addendum block

Write the PEC color count in N-1 with 0 becoming FF. Write the PEC
colors after that. Pad the block at 128 bytes of 0x20 (ASCII-32, space)
For each PEC color write: 0x90 elements (144) copies of 0. Write each
RGB color, in 3 byte increments blocks.

Write 2 bytes of 0. 0x00 0x00 (this only appears at V6).

## Write Truncated v.6

The valid header for a blank v.6 PES file is: 23 50 45 53 30 30 36 30 64
00 00 00 01 00 30 31 00 00 00 00 00 01 00 00 00 64 00 64 00 00 00 C8 00
C8 00 64 00 64 00 64 00 0C 00 13 00 01 00 00 00 00 00 19 00 00 00 00 00
00 00 00 80 3F 00 00 00 00 00 00 00 00 00 00 80 3F 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

This works but suffers from some rather notable oddities since the
colors are absent and expected. PE-Design loads blank as there's no data
for it. Wilcom loads with random colors, despite there still being valid
PEC colors in the PEC block. However, we could rectify that if we
realize that the reason for omitting the harder to write segment blocks
is that the segment value is zero. We could simply write in some threads
\*and\* then say there's no segments. In theory most readers will
iterate through and find the threads, and load those, then seek to the
PEC block. That makes it pretty easy to feed it correct data such a
reader would be looking for.

## Write PEC v.1

File should end with .pec Write "\#PEC0001" Continue to [Write
PEC](Embroidery_format_PEC#Writing_PEC "wikilink")

## Write Truncated PES v.1

  - Write \#PES0001
  - Write 1 byte 0x16
  - Write 13 bytes 0x00

Continue to [Write PEC](Embroidery_format_PEC#Writing_PEC "wikilink")

### Notes

The PES block, in total, is:

  - 23 50 45 53 30 30 30 31 16 00 00 00 00 00 00 00 00 00 00 00 00 00

No software other than PE-Design will notice, and PE-Design will load
normally and display a blank document.

## Writing Ultra-Trucated PES v.1

  - Write \#PES0001
  - Write 0x0c 0x00 0x00 0x00

Continue to [Write PEC](Embroidery_format_PEC#Writing_PEC "wikilink")

### Notes

This PES block is in total: "\#PES0001" 'u32' 12 (0C 00 00 00), the
truncated header is 12 bytes in total. The magic number tag and the 4
bytes that says the PEC file starts at the 12th (0x0C) byte.

PE-Design will crash, but no other software will notice that there's no
PES or header.

As a practical concern, most embroidery in .pes format does not actually
contain any of that design information data, it only contains the line
segment data, which is very difficult to edit, resize (significantly),
etc.

## PES Block Removing

The only thing that reads the PES block is PE-DESIGN which is
proprietary, Windows only, and expensive (like nearly all embroidery
software). So if the desire is to use Brother embroidery machines rather
than Brother embroidery software it would be sufficient to write a
minimal PES block a version 1 header and declare the start of the PEC
block just after the header and no segment-block elements:

`23 50 45 53 30 30 30 31 16 00 00 00 00 00 00 00 00 00 00 00 00 00`

`\# P E S 0 0 0 1 . . . . . . . . . . . . . . `

This is, \#PES0001 followed by a single byte 0x16 and then 13 zeros.
This is immediately followed by the PEC block. All software will read
this as correct. This is a valid PES block for a Version 1 blank
document. Only PE-DESIGN reads the PES block all other software will
read the PEC block only, to PE-DESIGN this is a valid blank document.

It is also possible truncate the header, and nearly no software will
realize there is an invalid header because most all software doesn't
read the PES header or anything about the PES part of the file:

`23 50 45 53 30 30 30 31 0C 00 00 00 `

`\# P E S 0 0 0 1 . . . . `

This is an invalid header, but no software other than PE-DESIGN will
realize it, because nearly all software only reads the file version
preamble, e.g. \#PES0001, then the 4 bytes for where it needs to seek
within the file to find the PEC block, and then the PEC block. Here we
simply declare it starts at the 12th byte, and omit the 10 zeros
additionals zeros which would make the header valid. Other software
won't notice, and PE-DESIGN will throw an error, rather than load a
blank document without objection.

## PEC Block Removal

Likewise PE-DESIGN does not read any of the PEC block data, with the
exception of a preview window in the open file dialog. You can set the
declared PEC location to garbage and remove all the PEC data from the
file and PE-DESIGN will not care.

Between the PES blocks between which there are two control codes 00 00
00 00 and FF FF 00 00 which mean END-OF-FILE and CONTINUE. It is the
case that after the 00 00 00 00 code, PE-DESIGN stops reading. It will
throw no error for any data beyond that section. And stops reading.
Between the 00 00 00 00 stop and the PEC block, you could put any data.
So long as the PES block is valid, and the PEC seek value is accurate,
you could have a valid .pec file with both a complete PEC block and a
complete PES block and a bunch of any data between them.

In special cases, where the design goes outside the the allowed hoop
within PE-Design the program itself will save the data without saving
the card data which leads to a blank PEC section. While it exists it
contains a name 0 colors (0xFF) a blank image, and some trailing zeros.

# PEC Section

The PEC format is read directly by embroidery machines. It is often
included in PES files in order to do the machine part of the code. If
the PEC file is standalone rather than embedded in the PES file it often
begins with the magic string "\#PEC0001" and then has the rest of the
file. PES files always start with \#PESxxxx then an exact reference to
the position of the PEC file within that file.

## PEC Header

### First PEC Section

The first part of the PEC header is 512 bytes.

| Type     | Bytes       | Value                                         | Description                                                                                              |
| -------- | ----------- | --------------------------------------------- | -------------------------------------------------------------------------------------------------------- |
| `char` | 19          |                                               | Label string prefixed with `"LA:"` and padded with space (`0x20`)                                    |
| `char` | 1           | `'\\r'`                                     | carriage return character                                                                                |
| `u8`   | 11          | `0x20` (typical)                            | Unknown                                                                                                  |
| `u8`   | 1           | `0x20` | `0xFF` | ?                       | Unknown                                                                                                  |
| `u16`  | 2           | `0x00FF` (typical)                          | Unknown                                                                                                  |
| `u8`   | 1           | `6` (typical)                               | Thumbnail image width in bytes, with 8 bit pixels per byte, so `6` would mean 6×8 = 48 pixels per line |
| `u8`   | 1           | `38` (typical)                              | Thumbnail image height in pixels                                                                         |
| `u8`   | 12          | `0x00` | `0x20` | `0x64` | `0xFF` | ? | Unknown, usually 20 20 20 20 64 20 00 20 00 20 20 20                                                     |
| `u8`   | 1           | _N_                                         | Number of colors minus one, 0xFF means 0 colors.                                                         |
| `u8`   | 1 + _N_   |                                               | Palette index                                                                                            |
| `u8`   | 462 - _N_ | `0x20` (typical)                            | Padding to second section.                                                                               |
|          |             |                                               |                                                                                                          |

PEC header

### Second PEC Section

The second part of the PEC header.

| Type                           | Bytes | Value                | Description                                                                            |
| ------------------------------ | ----- | -------------------- | -------------------------------------------------------------------------------------- |
| `u16`                        | 2     | `0x0000` (typical) | Unknown                                                                                |
| `u16`                        | 2     |                      | Offset to thumbnail image subsection relative to the PEC section offset plus 512 bytes |
| `u16`                        | 2     | `0x3100` (typical) | Unknown                                                                                |
| `u16`                        | 2     | `0xF0FF` (typical) | Unknown                                                                                |
| `s16`                        | 2     |                      | Width                                                                                  |
| `s16`                        | 2     |                      | Height                                                                                 |
| `u16`                        | 2     | `0x01E0` (typical) | Unknown                                                                                |
| `u16`                        | 2     | `0x01B0` (typical) | Unknown                                                                                |
|                                | 4     |                      | Unknown                                                                                |
| pec-stitch-list-subsection     |       |                      | PEC stitch list subsection                                                             |
| pec-thumbnail-image-subsection |       |                      | PEC thumbnail image subsection                                                         |
|                                |       |                      |                                                                                        |

PEC header

## PEC stitch list subsection

The PEC stitch list come in dx and dy relative coordinate pairs.
Depending on the bit patterns used, 2, 3 or 4 bytes are used for the
coordinate pairs as explained below. Note that if the first coordinates
are (0, 0) the embroidery machine apparently ignores them. The first
nonzero coordinate pair is relative to the minimum bound coordinates.

| Type   | Bytes  | Value | Description          |
| ------ | ------ | ----- | -------------------- |
| `u8` | 1 or 2 |       | Stitch dx coordinate |
| `u8` | 1 or 2 |       | Stitch dy coordinate |
|        |        |       |                      |

PES body

Each stitch we read the dx or dy coordinate. The topmost bit in a
coordinate is the flag for long form. If the bit is zero then the
following 7 bits are 2s complement binary for the dx or dy value. If the
long form is flagged then the next three bits there are control bits,
and the remaining 12 (including all 8 of the next bit) are overtly 12
bit 2s complement binary.

| 7 | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
| - | -- | -- | -- | -- | -- | -- | -- |
| 0 | b6 | b5 | b4 | b3 | b2 | b1 | b0 |
|   |    |    |    |    |    |    |    |

Short Form, 1 byte

In short form, the first most significant bit is 0 and the remaining
b6-b0 are 2s complement of a 7 bit number with a max range of 127.

| 7 | 6  | 5  | 4  | 3   | 2   | 1  | 0  | 7  | 6  | 5  | 4  | 3  | 2  | 1  | 0  |
| - | -- | -- | -- | --- | --- | -- | -- | -- | -- | -- | -- | -- | -- | -- | -- |
| 1 | c0 | c1 | c2 | b11 | b10 | b9 | b8 | b7 | b6 | b5 | b4 | b3 | b2 | b1 | b0 |
|   |    |    |    |     |     |    |    |    |    |    |    |    |    |    |    |

Long Form, 2 byte

In long form, the first most significant bit is 1. The following three
bits are then control codes, and the remaining b11-b0 are 2s complement
of a 12 bit number with a max range of 2047 in any extend.

## Commands

Since sort form can only encode stitches, all commands will always set
the long-form bit.

### Jump

Jumps are long form stitches with the command bit for Jump set.

| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
| - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - |
| 1 | 0 | 0 | 1 | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? |
|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |

Long Form, 2 byte

### Trim

Trim commands are long form stitches with the command bit for Trim set.

| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
| - | - | - | - | - | - | - | - | - | - | - | - | - | - | - | - |
| 1 | 0 | 1 | 0 | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? | ? |
|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |

Long Form, 2 byte

### Color Change

0xfe 0xb0 2, 1, 2, 1, 2, 1... alternating back and forth.

### END

File end is simply writing 0xFF.

### PEC thumbnail image subsection

There is always one main thumbnail image plus one for each color. The
size of each thumbnail is the thumbnail width multiplied with the
thumbnail height number of bytes (width and height are given in the PEC
section). The pixels are oriented from the top left corner, with one
pixel per bit starting with the most significant bit in the first byte.

### PEC thread palette
The following 64 threads have been verified with a Brother Innovis 955
sewing and embroidery machine and its “EMBROIDERY” thread space.

| Index | Code | Name              | Type | RGB color    |
| ----- | ---- | ----------------- | ---- | ------------ |
| 1     | 007  | Prussian Blue     | A    | `\#1a0a94` |
| 2     | 000  | Blue              | A    | `\#0f75ff` |
| 3     | 534  | Teal Green        | A    | `\#00934c` |
| 4     | 070  | Corn Flower Blue  | A    | `\#babdfe` |
| 5     | 800  | Red               | A    | `\#ec0000` |
| 6     | 000  | Reddish Brown     | A    | `\#e4995a` |
| 7     | 620  | Magenta           | A    | `\#cc48ab` |
| 8     | 810  | Light Lilac       | A    | `\#fdc4fa` |
| 9     | 000  | Lilac             | A    | `\#dd84cd` |
| 10    | 502  | Mint Green        | A    | `\#6bd38a` |
| 11    | 214  | Deep Gold         | A    | `\#e4a945` |
| 12    | 208  | Orange            | A    | `\#ffbd42` |
| 13    | 000  | Yellow            | A    | `\#ffe600` |
| 14    | 513  | Lime Green        | A    | `\#6cd900` |
| 15    | 328  | Brass             | A    | `\#c1a941` |
| 16    | 005  | Silver            | A    | `\#b5ad97` |
| 17    | 000  | Russet Brown      | A    | `\#ba9c5f` |
| 18    | 000  | Cream Brown       | A    | `\#faf59e` |
| 19    | 704  | Pewter            | A    | `\#808080` |
| 20    | 900  | Black             | A    | `\#000000` |
| 21    | 000  | Ultramarine       | A    | `\#001cdf` |
| 22    | 000  | Royal Purple      | A    | `\#df00b8` |
| 23    | 707  | Dark Gray         | A    | `\#626262` |
| 24    | 058  | Dark Brown        | A    | `\#69260d` |
| 25    | 086  | Deep Rose         | A    | `\#ff0060` |
| 26    | 323  | Light Brown       | A    | `\#bf8200` |
| 27    | 079  | Salmon Pink       | A    | `\#f39178` |
| 28    | 000  | Vermilion         | A    | `\#ff6805` |
| 29    | 001  | White             | A    | `\#f0f0f0` |
| 30    | 000  | Violet            | A    | `\#c832cd` |
| 31    | 000  | Seacrest          | A    | `\#b0bf9b` |
| 32    | 019  | Sky Blue          | A    | `\#65bfeb` |
| 33    | 000  | Pumpkin           | A    | `\#ffba04` |
| 34    | 010  | Cream Yellow      | A    | `\#fff06c` |
| 35    | 000  | Khaki             | A    | `\#feca15` |
| 36    | 000  | Clay Brown        | A    | `\#f38101` |
| 37    | 000  | Leaf Green        | A    | `\#37a923` |
| 38    | 405  | Peacock Blue      | A    | `\#23465f` |
| 39    | 000  | Gray              | A    | `\#a6a695` |
| 40    | 000  | Warm Gray         | A    | `\#cebfa6` |
| 41    | 000  | Dark Olive        | A    | `\#96aa02` |
| 42    | 307  | Linen             | A    | `\#ffe3c6` |
| 43    | 000  | Pink              | A    | `\#ff99d7` |
| 44    | 000  | Deep Green        | A    | `\#007004` |
| 45    | 000  | Lavender          | A    | `\#edccfb` |
| 46    | 000  | Wisteria Violet   | A    | `\#c089d8` |
| 47    | 843  | Beige             | A    | `\#e7d9b4` |
| 48    | 000  | Carmine           | A    | `\#e90e86` |
| 49    | 000  | Amber Red         | A    | `\#cf6829` |
| 50    | 000  | Olive Green       | A    | `\#408615` |
| 51    | 107  | Dark Fuchsia      | A    | `\#db1797` |
| 52    | 209  | Tangerine         | A    | `\#ffa704` |
| 53    | 017  | Light Blue        | A    | `\#b9ffff` |
| 54    | 507  | Emerald Green     | A    | `\#228927` |
| 55    | 614  | Purple            | A    | `\#b612cd` |
| 56    | 515  | Moss Green        | A    | `\#00aa00` |
| 57    | 124  | Flesh Pink        | A    | `\#fea9dc` |
| 58    | 000  | Harvest Gold      | A    | `\#fed510` |
| 59    | 000  | Electric Blue     | A    | `\#0097df` |
| 60    | 205  | Lemon Yellow      | A    | `\#ffff84` |
| 61    | 027  | Fresh Green       | A    | `\#cfe774` |
| 62    | 000  | Applique Material | A    | `\#ffc864` |
| 63    | 000  | Applique Position | A    | `\#ffc8c8` |
| 64    | 000  | Applique          | A    | `\#ffc8c8` |
|       |      |                   |      |              |

PES colors

# Writing PEC

If the PEC block is part of a PES file, after PESv4, the advent of
thread descriptions in the PES header means the color codes are
overloaded rather than use the magic numbers lookup chart. Most all
loaders will use a hybrid of the PES header for the colors, and the PEC
block for the color values.

## Write PEC Elements

If we are writing a basic output into PEC format, we should do the
following:

  - Write "LA:"
  - Write 20 bytes, name, this is typically truncated at 8 characters,
    padded with space (0x20)
  - Write 1 byte, 0x0D
  - Write 12 bytes, 12 copies of 0x20
  - Write 1 byte, 0xFF
  - Write 1 byte, 0x00
  - Write 1 byte, 0x06, Graphics size information.
  - Write 1 byte, 0x26, Graphics size information.
  - Write 12 bytes, 0x20, 12 spaces.
  - Write 1 byte, \#Color_Changes, (n) number of color changes.
  - Write n bytes, \#Index Color each change.
  - Write (463-n) spaces, that is to say pad color changes to 463 in
    total.

(This is 512 bytes from the start of the PEC FILE)

  - Write 2 bytes, 0x00 0x00, two bytes of zeros.
  - Write 3 bytes, (NOTE: 3 bytes) position of graphics offset value.
    (20 + PEC_ENCODE)
  - Write 1 byte: 0x31
  - Write 1 byte: 0xFF
  - Write 1 byte: 0xF0
  - Write 2 bytes, Width.
  - Write 2 bytes, Height.
  - Write 2 bytes, 0x01E0
  - Write 2 bytes, 0x01B0
  - Write 2 bytes BIGENDIAN 0x9000 - minX
  - Write 2 bytes BIGENDIAN 0x9000 - minY
  - Write Pec Encoded stitches.
  - Write Graphics Objects.

## Write Pec Encoded Stitches.

Though it might be technically right and there are documents with mixed
length stitches, if the X or Y is long form they both should be. Just as
both X and Y get flagged for jumps and trims, when we jump and trim.
It's unknown which of these are technically valid and on which machines.
But, both doing this consistently seems to be maximally compatible.

### Stitch

Write stitches below 127 in short form.

Short form stitches are 2 bytes. X, Y. These are 7 bit signed integers.
0b0???????, 0b0???????

Write stitches above 127 in long form. 0b1000????_????????,
0b1000????_???????? the topmost nibble is command data, and the bottom
12 bits are coordinate data.

Jumps are long form stitches with the command bit for Jump set.
0b1001????_????????, 0b1001????_????????

Trim commands are long form stitches with the command bit for Trim set.
0b1010????_????????, 0b1010????_????????

Color changes should write: Write 1 byte: 0xfe Write 1 byte: 0xb0 Write
1 byte: 2, 1, 2, 1, 2, 1... alternating back and forth. Starting with 2
and each additional color change use the next character in sequence.

For END write: Write 1 byte: 0xFF

## Write Graphics

  - 1 copy of blank.
  - N copies of blank. (where N is the number of colors used).

### Blank Graphics

(byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte)
0x00, (byte) 0xF0, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF,
(byte) 0x0F, (byte) 0x08, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte)
0x00, (byte) 0x10, (byte) 0x04, (byte) 0x00, (byte) 0x00, (byte) 0x00,
(byte) 0x00, (byte) 0x20, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte)
0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00, (byte) 0x00,
(byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00, (byte)
0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00,
(byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte)
0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02,
(byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte)
0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40,
(byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte)
0x40, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
(byte) 0x40, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte)
0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00,
(byte) 0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte)
0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00, (byte) 0x00,
(byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00, (byte)
0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00,
(byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte)
0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02,
(byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte)
0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40,
(byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte)
0x40, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
(byte) 0x40, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte)
0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00,
(byte) 0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte)
0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00, (byte) 0x00,
(byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00, (byte)
0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte) 0x00,
(byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02, (byte)
0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte) 0x02,
(byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40, (byte)
0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x40,
(byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte)
0x40, (byte) 0x04, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
(byte) 0x20, (byte) 0x08, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte)
0x00, (byte) 0x10, (byte) 0xF0, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF,
(byte) 0xFF, (byte) 0x0F, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte)
0x00, (byte) 0x00, (byte) 0x00

## Consideration of the Wilcom Bug

A bug in Wilcom's reading of how to make this valid leads to two
different interpretations.

Properly in PE-Design, if you reuse the same thread it doesn't add
another thread entry to the thread table at the start. Rather it lists
the unique threads. So if you used black, white, black, white, red,
black, white. Brother would say you used 3 threads and define them in
the header. In the PEC file it would list these 14 1D 14 1D 05 14 1D.
Assigning the first thread value to 14, the second to 1D, and the third
finally to 05.

Wilcom however, loads the thread table as three threads black, white,
red, then completely ignores the order listed in the PEC table and uses
them in a cycle coloring them black, red, white, black, red, white,
black. This bug is easily duplicated.

When saving PE-Design will save the unique threads. When saving Wilcom
will save all threads, in a list, repeating them as needed. And it will
pigeon hole colors into being the nearest relevant color, when writing
the PEC. However, Wilcom writes the PES file with the table colors
referring to the header, so PE-Design loads these correctly. However
they are previewed by PE-Design as wrong when you check the Open Dialog.

So when these colors get pigeon-holed into the same PECThread index
value but refer to different colors, PE-Design will interpret them
incorrectly, in the rare preview of the stitches. However, if we only
provide the unique colors table as PES properly requires, Wilcom will
read these in error if there a duplicate color, prior to exhausting all
unique colors.

The embroidery machine will use those PEC colors for the preview
screens, and accept that when a color change happens that it moves to
the next needle. If \#17 is the closest color to the first thread, so it
calls that 17. And \#17 is closest color to the second thread, it should
not use \#17 again. But, properly PE-Design takes \#17 to always refer
to the same thread no matter what. And Wilcom won't care, and just cycle
to the next thread in it's chart.

The problem here is a bit of a catch-22. If you read/write the files
properly, there will be a rather notable incompatibility with Wilcom.
And it'll work fine with PE-Design. If you read/write the files in
Wilcom's way, PE-Design will load the PES blocks fine and seem to work
without issue outside of preview on load, and Wilcom will also work
correctly for the most part, even though it's technically still wrong
and will suffer in a few minor ways.

For max compatibility export the correct PES file with unique threads in
the header, and detect the error when the number of threads is equal to
the number of colors listed in the pec section. If the threads in the
header is equal to the colors indexed in the PEC section, then use 1 to
1, if there are fewer colors in the header, properly index them as
PE-Design expects.

# Credits

This file and the accompanied PES file, give credit to:

  - A lot of this information is consolidated from [Frno7's
    Wiki](https://github.com/frno7/libpes/wiki/PES-format) which I did a
    lot of work on.
  - As well as some rather important work by JoshVarga from
    Embroidermodder.
  - [treveradams](https://github.com/treveradams/libpes)
  - This old site, <http://www.achatina.de/sewing/main/TECHNICL.HTM>
  - Some early work done on the graphics decoding by Linus Torvalds
    (yes, that one).
