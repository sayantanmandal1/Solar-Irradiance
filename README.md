# ☀️ Solar Irradiance Estimation using LDRs & Machine Learning

A low-cost, scalable, and intelligent system to **estimate solar irradiance** using **Light Dependent Resistors (LDRs)** powered by **Machine Learning**. This project bridges the gap between affordability and accuracy, enabling real-time irradiance monitoring for **solar energy systems**, **environmental tracking**, and **climate research**.

---

## 📌 Project Overview

Traditional solar measurement tools like pyranometers are expensive and bulky. This project introduces a **cost-effective solution** by using simple LDR sensors combined with ML models to predict irradiance levels with high precision.

> 🌱 **Goal**: Transform raw LDR data into calibrated irradiance readings using supervised ML algorithms.

---

## 🔄 Workflow

### 1️⃣ Data Collection
- Multiple LDRs placed under varied lighting conditions (different times, weather).
- Microcontroller logs raw analog values.

### 2️⃣ Data Preprocessing
- Normalization & cleaning.
- Optional inclusion of temperature/humidity for enhanced model accuracy.

### 3️⃣ Machine Learning
- Models: Linear Regression, Decision Trees, Neural Networks.
- Training using labeled pyranometer reference data.

### 4️⃣ Evaluation & Optimization
- Metrics: MAE, RMSE.
- Hyperparameter tuning to boost performance.

### 5️⃣ Deployment
- Deployable on microcontrollers (Arduino/Raspberry Pi).
- Real-time monitoring via scripts or dashboard.

---

## 📈 Results

✅ High-accuracy irradiance prediction from LDR inputs.  
✅ MAE and RMSE within acceptable industry ranges.  
✅ Operational on low-power hardware.  
✅ Scalable for distributed sensor networks.

---

## 🧰 Technologies Used

| Category | Tools/Components |
|---------|------------------|
| **Hardware** | LDR Sensors, Arduino / Raspberry Pi |
| **Languages** | Python, C++ |
| **Libraries** | Scikit-learn, TensorFlow/Keras |
| **Data Interface** | Serial or IoT dashboard (optional) |

---

## 📦 Repository Structure

```
solar-irradiance-estimation/
├── data_collection.py            # Script to gather LDR sensor data
├── train_model.py                # Train machine learning models
├── real_time_inference.py       # Deployable script for real-time use
├── hardware_setup.md            # Wiring diagrams and hardware guide
├── requirements.txt             # Python dependencies
├── README.md                    # Project documentation
```

---

## ⚙️ Installation & Setup

```bash
git clone https://github.com/sayantanmandal1/solar-irradiance-estimation.git
cd solar-irradiance-estimation
pip install -r requirements.txt
```

Connect LDR sensors as per `hardware_setup.md` and begin data collection.

---

## 🚀 Usage

### 📡 Data Collection
```bash
python data_collection.py
```

### 🧠 Train the Model
```bash
python train_model.py
```

### 🌞 Real-Time Estimation
Upload and run `real_time_inference.py` on your microcontroller for continuous monitoring.

---

## 📊 Sample Results

```
Input (LDR):  853, 871, 822
Predicted Irradiance:  742.3 W/m²
Reference Pyranometer Reading: 748.0 W/m²
Error: 0.76%
```

---

## 🤝 Contributing

Have an idea or improvement? Open a PR or issue. Let's build smarter solar systems together!

---

## 📜 License

This project is under the **MIT License**. See `LICENSE` for full details.

---

## 🧠 Author

**Sayantan Mandal**  
Passionate about Embedded Systems, IoT, and Sustainable Tech 🌍

> For academic collaborations or enhancements, feel free to reach out!

---

## 🌐 Applications

- Smart Solar Farms 🌾
- Renewable Energy Analytics ⚡
- Urban Environmental Monitoring 🌇
- Educational and Research Tools 🎓

---

> Designed with ☀️ to empower **Green Energy Innovation**
