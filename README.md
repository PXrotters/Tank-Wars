# 🚀 Tank Wars – OpenGL Game  

📌 **Autor**: Lemnaru Mihai-Daniel  
📌 **Tehnologii**: C++, OpenGL  
📌 **Framework recomandat**: Visual Studio  - gfxframework

---

## 🎯 Descriere  

**Tank Wars** este un joc inspirat de celebrul titlu clasic, dar cu un **gameplay interactiv** și un teren **dinamic** și **deformabil**. Jucătorii controlează tancuri care se deplasează pe un teren generat procedural și își atacă inamicii folosind proiectile balistice.  

---

## 📂 Structura Proiectului  

- **`src/lab_m1/Tema1/`** – Implementarea principală  
  - `Tema1.cpp` – Codul sursă principal  
  - `Tema1.h` – Header-ul principal  
- **`assets/`** – Resurse grafice și shader-e  
- **`deps/`** – Dependențe (OpenGL, GLFW, GLM)  
- **`build/`** – Fișiere de compilare (se exclude din arhivare)  

---

## 🎮 Funcționalități  

### 🏔️ **Teren Deformabil**  
✔️ Generare procedurală a terenului cu **sinusoide și curbe complexe**  
✔️ Stocare în **height map**  
✔️ **Deformare dinamică** la impactul proiectilelor  

### 🚜 **Tancuri Mobile și Orientare Dinamică**  
✔️ Se aliniază automat la înclinația terenului  
✔️ Se pot **deplasa stânga-dreapta** respectând conturul solului  
✔️ **Turela se rotește** și poate trage proiectile  

### 💥 **Proiectile și Coliziuni**  
✔️ Simulare fizică a traiectoriei cu **gravitație**  
✔️ **Coliziuni detectate** cu tancurile și terenul  
✔️ **Explozie și deformare** a terenului la impact  

### 🎯 **Interfață și Feedback Vizual**  
✔️ **Bară de viață** deasupra tancurilor  
✔️ **Previzualizare traiectorie** înainte de tragere  

---

## 🕹️ Controale  

- **A / D** – Deplasare tanc  
- **W / S** – Rotire turelă  
- **Space** – Lansare proiectil  
- **Esc** – Ieșire din joc  

---

## ✅ Funcționalități Avansate
✔️ Alunecare de teren în zonele abrupte
✔️ Efecte vizuale și animații
✔️ Traiectorie predictivă a proiectilului

---
