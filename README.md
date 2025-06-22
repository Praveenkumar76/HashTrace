for vedio refernce it didnt work why ?
# 🔍 HashTrace - Code Plagiarism Detector

HashTrace is a modern and efficient plagiarism detection system built using **Qt (QML)** for the frontend and **C++** as the backend. It uses the **Rabin-Karp rolling hash algorithm** to identify code/text similarities between files and highlights them with an intuitive interface.

![HashTrace Banner](assets/banner.png) 

---

## 📽 Demo

▶️ **Watch the demo video here**:  
[![HashTrace Demo](assets/HashTrace_Output.mp4)]

> *(Replace `VIDEO_ID_HERE` with the actual YouTube video ID after uploading)*

---

## ✨ Features

- 📁 Drag-and-drop support for adding files
- 🧠 Rabin-Karp algorithm for efficient hash-based comparison
- 🔍 Side-by-side file comparison with highlighted duplicate content
- 📊 Similarity score between files
- 💡 Minimal, responsive UI using QML
- 🧹 Handles preprocessing like removing comments and reserved keywords

---

## ⚙️ How It Works

1. **Preprocessing**: Cleans the source code (removes comments, blank lines, reserved words).
2. **Hashing**: Uses Rabin-Karp rolling hash to generate fingerprints.
3. **Comparison**: Compares hashes and highlights matched segments.
4. **Visualization**: Displays the results with matching blocks and similarity percentage.

---

## 🧰 Tech Stack

- **Frontend**: Qt 6, QML
- **Backend**: C++17
- **Build System**: CMake
- **Algorithm**: Rabin-Karp Rolling Hash

---

## 🖥️ Installation

```bash
# Clone the repository
git clone https://github.com/your-username/HashTrace.git
cd HashTrace

# Build the project
mkdir build && cd build
cmake ..
make
```

## 🚀 Run the App
```bash
./HashTrace
```

## 📄 License
- This project is licensed under the MIT License.

## 🙋‍♂️ Author
Praveen Kumar
praveenrajagopal45@gmail.com
