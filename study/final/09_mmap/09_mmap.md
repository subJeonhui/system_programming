# mmap

---
### Process Address Space  
- Text Area (code)
  - ROM에 저장 const int x = 100;
- Data Area 
  - global initialized data & global uninitilaized data  
  (bss = block started by symbol)
- Heap Area (dynamic 동적)
  - malloc(), new, etc.
- Stack Area (Automatic 자동
  - 함수 호출시 그 함수를 위한 frame 할당
---
### Memory Mapped File
