#ifndef THRED_MOUSE_H
#define THRED_MOUSE_H

namespace mouse {
void crtcurs() noexcept;
void dstcurs() noexcept;
auto getArrowCursor() noexcept -> HCURSOR;
auto handleEitherButtonDown() -> bool;
auto handleLeftButtonDown(std::vector<POINT>& stretchBoxLine,
                          float&              xyRatio,
                          FRM_HEAD const&     textureForm,
                          bool&               retflag) -> bool;
auto handleLeftButtonUp(float xyRatio, float rotationAngle, F_POINT& rotationCenter, bool& retflag) -> bool;
auto handleMouseMove(std::vector<POINT>& stretchBoxLine,
                     float               xyRatio,
                     float&              rotationAngle,
                     F_POINT const&      rotationCenter,
                     FRM_HEAD const&     textureForm) -> bool;
auto handleRightButtonDown() -> bool;
} // namespace mouse

#endif // THRED_MOUSE_H
