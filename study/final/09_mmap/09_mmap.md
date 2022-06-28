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

#### 메모리 영역을 파일로 mapping하여

- 실행 시 파일에 메모리 내 변수들을 자동 저장
- Memory reading = reading from a file
- Memory writing = writing to a file  
  (page cache 이용 disk로 바로 내려가지 않음 [delay write])
- library buffer와 kernel buffer에 복사 X

---
<pre><code>#include &lt;sys&#47;types.h&gt;
#include &lt;sys&#47;mman.h&gt;
caddr_t mmap (caddr_t addr, size_t len, int prot,
               int flags, int fd, off_t offset)
</code></pre>

|---|---|---|---|
| caddr_t addr | |  
| size_t len | |
| int pro t ||
| int flags ||
| int fd ||
| off_t offset ||