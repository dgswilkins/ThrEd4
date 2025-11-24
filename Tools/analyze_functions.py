#!/usr/bin/env python3
"""
Script to analyze C++ functions in thred.cpp and generate a markdown table
showing function names and their lengths (in lines of code).
"""

import re
from pathlib import Path
from typing import List, Tuple

def count_lines_in_function(lines: List[str], start_idx: int) -> int:
    """
    Count lines in a function starting from the opening brace.
    Returns the number of lines including the closing brace.
    """
    brace_count = 0
    line_count = 0
    in_function = False
    
    for i in range(start_idx, len(lines)):
        line = lines[i]
        # Count opening and closing braces
        brace_count += line.count('{')
        brace_count -= line.count('}')
        
        if brace_count > 0:
            in_function = True
        
        if in_function:
            line_count += 1
        
        if in_function and brace_count == 0:
            break
    
    return line_count

def extract_functions(file_path: str) -> List[Tuple[str, int, int]]:
    """
 Extract function names and their lengths from a C++ file.
    Returns list of tuples: (function_name, line_number, line_count)
    """
    functions = []
    
    with open(file_path, 'r', encoding='utf-8') as f:
        lines = f.readlines()
    
    # Pattern to match function definitions
    # Matches: return_type function_name(params) [qualifiers] {
    function_pattern = re.compile(
   r'^(?:auto|void|bool|int|uint32_t|uint16_t|uint8_t|float|double|LRESULT|INT_PTR|BOOL|HPEN|POINT|F_POINT|COLORREF|wchar_t|size_t|fs::path)\s+'
        r'(?:(?:CALLBACK|thred::)\s+)?'
 r'(\w+)\s*\([^)]*\)'
        r'(?:\s+(?:const|noexcept|override|->.*?))?\s*{'
    )
    
    for i, line in enumerate(lines):
        match = function_pattern.search(line)
        if match:
            function_name = match.group(1)
            line_count = count_lines_in_function(lines, i)
            functions.append((function_name, i + 1, line_count))
    
    return functions

def generate_markdown_table(functions: List[Tuple[str, int, int]], output_file: str):
    """
    Generate a markdown table with function analysis.
    """
    # Sort by line count (descending)
    sorted_functions = sorted(functions, key=lambda x: x[2], reverse=True)
    
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("# ThrEd4 Function Analysis\n\n")
        f.write("## Summary Statistics\n\n")
        
        total_functions = len(functions)
        total_lines = sum(func[2] for func in functions)
        avg_lines = total_lines / total_functions if total_functions > 0 else 0
        max_lines = max((func[2] for func in functions), default=0)
        min_lines = min((func[2] for func in functions), default=0)
        
        f.write(f"- **Total Functions**: {total_functions}\n")
        f.write(f"- **Total Lines of Code**: {total_lines:,}\n")
        f.write(f"- **Average Function Length**: {avg_lines:.1f} lines\n")
        f.write(f"- **Longest Function**: {max_lines} lines\n")
        f.write(f"- **Shortest Function**: {min_lines} lines\n\n")
        
        # Functions by size category
        small = sum(1 for func in functions if func[2] < 50)
        medium = sum(1 for func in functions if 50 <= func[2] < 200)
        large = sum(1 for func in functions if 200 <= func[2] < 500)
        very_large = sum(1 for func in functions if func[2] >= 500)
        
        f.write("### Functions by Size\n\n")
        f.write(f"- **Small** (< 50 lines): {small}\n")
        f.write(f"- **Medium** (50-199 lines): {medium}\n")
        f.write(f"- **Large** (200-499 lines): {large}\n")
        f.write(f"- **Very Large** (≥ 500 lines): {very_large}\n\n")

        f.write("## All Functions (Sorted by Length)\n\n")
        f.write("| Rank | Function Name | Line Number | Length (lines) |\n")
        f.write("|------|---------------|-------------|----------------|\n")
        
        for rank, (name, line_num, line_count) in enumerate(sorted_functions, 1):
            f.write(f"| {rank} | `{name}` | {line_num} | {line_count} |\n")
        
        f.write("\n## Functions by Definition Order\n\n")
        f.write("| Function Name | Line Number | Length (lines) |\n")
        f.write("|---------------|-------------|----------------|\n")
            
        for name, line_num, line_count in functions:
            f.write(f"| `{name}` | {line_num} | {line_count} |\n")

def main():
    """Main entry point."""
    file_path = Path("D:/ThrEd4/ThrEd4/thred.cpp")
    output_file = "thredAnalysis.md"
    
    if not file_path.exists():
        # Try relative path
        file_path = Path("thred.cpp")
    if not file_path.exists():
        print(f"Error: Could not find thred.cpp file")
        return
    
    print(f"Analyzing {file_path}...")
    functions = extract_functions(str(file_path))
    
    print(f"Found {len(functions)} functions")
    print(f"Generating {output_file}...")
 
    generate_markdown_table(functions, output_file)
    
    print(f"? Analysis complete! Results saved to {output_file}")

if __name__ == "__main__":
    main()
