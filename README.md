<h3 align="center" style="font-size: 2.1em; font-weight: bolder;">FogOS</h3>

  <p align="center">
    An Operating System Built on Top of Xv6
    <br />
    <a href="https://github.com/nestrada2/FogOS/tree/main/docs"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/nestrada2/FogOS/issues/new?labels=bug&template=bug-report.md">Report Bug</a>
    ·
    <a href="https://github.com/nestrada2/FogOS/issues/new?labels=enhancement&template=feature-request.md">Request Feature</a>
  </p>
</div>

<!-- ABOUT THE PROJECT -->
## 📖 About the Project
![FogOS](docs/fogos.gif)

This project builds on top of the foundational xv6 operating system, a teaching OS developed by MIT based on Unix Version 6, serving as a hands-on tool for understanding OS internals, experimenting with kernel-level programming, and exploring Unix-like system development in a manageable and accessible codebase.

### **✨ Features**
- [test: check file types and compare values](https://github.com/USF-OS/FogOS/pull/77)
- [tosh: command line shell](https://github.com/nestrada2/FogOS/blob/main/docs/TOSH.md)

### **🛠️ Tech Stack**
[![C][C.com]][C-url]
<br />
[![Unix-like][Unix-like.com]][Unix-like-url]
<br />
[![Xv6][Xv6.com]][Xv6-url]
<br />
[![Qemu][Qemu.com]][Qemu-url]
<br />

<!-- GETTING STARTED -->
## 📦 Getting Started

### **💾 Installation**
1. **Install QEMU**
   <br />
   QEMU is required to emulate and test FogOS on your local machine. https://www.qemu.org/download/
2. **Clone the repo**
    ```sh
    git clone https://github.com/nestrada2/FogOS.git
    ```

### **▶️ Running the Program**
1. **Build FogOS**
   ```sh
   make
   ```
2. **Run FogOS with QEMU**
   ```sh
   make qemu
   ```

<!-- LICENSE -->
## 📜 License
Distributed under the xv6 License. See [`xv6-LICENSE`](xv6-LICENSE) for more information.

<!-- RESOURCES -->
## 📚 Resources
[Man](https://www.man7.org/linux/man-pages/index.html), 
[QEMU](https://www.qemu.org/docs/master/),
[Stack Overflow](https://stackoverflow.com/),
[W3 School](https://www.w3schools.com/),
[Geeks for Geeks](https://www.geeksforgeeks.org/)

<!------- MARKDOWN LINKS & IMAGES ------->
[C.com]: https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white
[C-url]: https://www.iso.org/standard/74528.html
[Unix-like.com]: https://img.shields.io/badge/Unix-like-4285F4?style=for-the-badge&logo=unix-like
[Unix-like-url]: #
[Xv6.com]: https://img.shields.io/badge/Xv6-100000?style=for-the-badge&logo=xv6
[Xv6-url]: https://pdos.csail.mit.edu/6.828/2012/xv6.html
[Qemu.com]: https://img.shields.io/badge/Qemu-EE0000?style=for-the-badge&logo=qemu
[Qemu-url]: https://www.qemu.org/
