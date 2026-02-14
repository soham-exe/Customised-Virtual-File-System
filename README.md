# Custom Virtual File System (CVFS)

## 🚀 Overview
CVFS is a monolithic, non-persistent virtual file system implemented in **CPP**. It simulates the core functionality of the Linux File System (ext2/ext3 architecture) completely in memory (RAM).

It provides a custom shell interface to interact with files using standard system call simulations like `open`, `read`, `write`, `lseek`, and `stat`. This project demonstrates advanced concepts of **System Programming**, **Data Structures**, and **OS Internals**.

## 🛠️ Tech Stack
* **Language:** CPP
* **Concepts:** Inodes, File Tables, DILB (Disk Inode List Block), UAREA, Super Block.
* **Interface:** Custom Command Line Shell.

## ✨ Key Features
* **Virtual Storage:** Implements an in-memory `SuperBlock` and `Inode` table to manage resources.
* **Custom Shell:** A continuous listening shell that accepts command-line arguments.
* **System Call Simulation:**
    * `creat`: Create new files with specific permissions (Read/Write).
    * `write`: Write data into files (updates file size & offset).
    * `read`: Read data from files (handles offsets & permissions).
    * `lseek`: Reposition the read/write cursor (`START`, `CURRENT`, `END`).
* **File Metadata:**
    * `stat` / `fstat`: Retrieve file details like Inode number, File Size, Link Count, and Type.
    * `ls`: List all files in the virtual directory.
* **Memory Management:** Dynamic allocation and deallocation of file blocks and inodes using `malloc` and `free`.

## 📖 Supported Commands
| Command | Usage | Description |
| :--- | :--- | :--- |
| `help` | `help` | Display all the supported commands |
| `man` | `man [cmd]` | Display manual for a command |
| `creat` | `creat [name] [perm]` | Create a file (Perm: 1=R, 2=W, 3=RW) |
| `write` | `write [fd] [size]` | Write data to a file descriptor |
| `read` | `read [fd] [size]` | Read data from a file descriptor |
| `lseek` | `lseek [fd] [off] [whence]` | Move cursor (0=Start, 1=Curr, 2=End) |
| `stat` | `stat [name]` | Show file info by name |
| `fstat` | `fstat [fd]` | Show file info by FD |
| `unlink`| `unlink [name]` | Delete a file |
| `ls` | `ls` | List all files |
| `clear` | `clear` | Clear terminal |
| `exit` | `exit` | Terminate the system |

## ⚙️ How to Run
1.  **Clone the repository**
    ```bash
    git clone [https://github.com/YOUR_USERNAME/CVFS-Project.git](https://github.com/YOUR_USERNAME/CVFS-Project.git)
    cd CVFS-Project
    ```

2.  **Compile the code**
    ```bash
    g++ src/cvfs.c -o cvfs
    ```

3.  **Run the application**
    * **Linux/Mac:** `./cvfs`
    * **Windows:** `cvfs.exe`

## 🧩 Architecture Details
* **Inodes:** Each file is represented by an Inode structure holding metadata (Size, Permissions, Type).
* **File Table (UFDT):** A User File Descriptor Table maps numeric File Descriptors (FDs) to actual Inode pointers.
* **Linked List:** A Singly Linked List is used to manage the Inodes for efficient traversal.

## 👨‍💻 Author
**Soham Sachin Sonar**
* [GitHub](https://github.com/soham-exe)
