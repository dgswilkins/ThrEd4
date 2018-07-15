#pragma once
void PolylineInt(HDC hdc, const POINT* apt, size_t cpt);
void WriteFileInt(HANDLE file, LPCVOID buffer, size_t bytesToWrite, LPDWORD bytesWritten, LPOVERLAPPED overlapped);
void GetTextExtentPoint32Int(HDC hdc, LPCTSTR lpString, size_t c, LPSIZE lpSize);
void TextOutInt(HDC hdc, int nXStart, int nYStart, LPCTSTR lpString, size_t cchString);
bool ReadFileInt(HANDLE file, LPVOID buffer, size_t bytesToRead, LPDWORD bytesRead, LPOVERLAPPED overlapped);
void GetTextExtentPointInt(HDC hdc, LPCTSTR lpString, size_t cbString, LPSIZE lpSize);