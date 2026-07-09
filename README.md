# LepNSI_analysis

# Neutrino Leptonic NSI Dashboard

An interactive dashboard for studying the sensitivity of future electron-positron colliders to **Leptonic Non-Standard Neutrino Interactions (NSI)** through the monophoton process

\[
e^-e^+ \rightarrow \gamma + E_T^{\rm miss}.
\]

The dashboard provides interactive visualization of

- Signal and background kinematic distributions
- Cross-section as a function of mediator mass
- NSI sensitivity (3σ limits)
- Collider comparison
- Beam polarization comparison
- Different operating scenarios
- Different mediator widths

The application is written in **Python** using **Dash**, **Plotly**, **Pandas**, and **Uproot**, while the analysis histograms are generated using **ROOT**.

---

# Features

- Interactive histogram viewer
- Interactive cross-section plots
- Interactive NSI sensitivity plots
- Multiple collider support
    - ILC
    - CLIC
    - FCC-ee
- For each collider
    - Beam polarization selection
    - Multiple mediator masses
    - Multiple mediator widths
    - Linear / logarithmic axis switches
    - Operating scenario comparison
    - Comparison of:
        - Pure Signal
        - Signal + Interference
    - Responsive web interface
---

# Repository Structure

```
NSI_Dashboard/

│
├── app.py                   # Main Dash application
|
├── src                     
│   ├── callbacks.py         # Dash callbacks
|   ├── layout.py            # Dashboard layout
|   └── plots.py             # Plot generation
|
|
├── assets/
│   ├── style.css            # Custom CSS
│   └── logos/
│
├── output/                  # Datasets
│   ├── ILC/
│   ├── CLIC/
│   └── FCC/
│
├── requirements.txt 
├── LICENSE 
└── README.md
```

---

# Installation

Create a Python environment

```bash
conda create -n NSI python=3.10
conda activate NSI
```

Install the required packages

```bash
pip install dash
pip install dash-bootstrap-components
pip install plotly
pip install pandas
pip install numpy
pip install uproot
```

or

```bash
pip install -r requirements.txt
```

---

# Dependencies

Python packages

- dash
- dash-bootstrap-components
- plotly
- pandas
- numpy
- uproot

Analysis software

- ROOT ≥ 6
- Delphes
- MadGraph5_aMC@NLO (for event generation)

Optional

- Pythia8
- FastJet

---

# Running the Dashboard

From the project directory

```bash
python app.py
```

The dashboard will start at

```
http://127.0.0.1:8050
```

Open this address using any modern browser.

---

# Using the Dashboard

The dashboard consists of four main panels.

## Input Controls

Users can choose

- Collider
- Beam polarization
- Mediator width
- Mediator mass
- Observable

depending on the collider selected.

---

## Histogram

Displays normalized distributions of

- Photon Energy
- Photon Polar Angle

for

- Signal
- Full Signal
- νν Background
- Bhabha Background

The toolbar allows

- Log X
- Log Y
- Grid

---

## Cross Section

Displays

\[
\sigma(e^-e^+\rightarrow\gamma+\nu\bar{\nu})
\]

as a function of mediator mass.

Shows

- Pure signal
- Signal including interference

---

## NSI Sensitivity

Displays the projected

3σ sensitivity

for

\[
|\epsilon_{\alpha\beta}^{ee}|
\]

The user can choose

- Operating scenario
- With / without systematic uncertainties
- Log scale

---

# Data Generation

The dashboard does **not** perform event simulation.

All input data are generated externally.

The workflow is

```
MadGraph
      ↓
Pythia
      ↓
Delphes
      ↓
ROOT analysis macro
      ↓
Dashboard
```

The ROOT analysis macro

- applies event selection
- fills histograms
- normalizes histograms
- stores them in ROOT format

Example output

```
output/

ILC/

    histo_W10M_0.root
    histo_W10M_1.root
    ...

    Contour_Pure.dat
    Contour_Full.dat

    CS_Pure.txt
    CS_Full.txt
```

---

# ROOT Histogram Format

The dashboard expects

```
Signal/
    M1/
        PhotonE
        CosTheta

    M100/
        PhotonE
        CosTheta

...

NuNu/
    PhotonE
    CosTheta

Bhabha/
    PhotonE
    CosTheta

Full/
    M1/
        PhotonE
        CosTheta
```

---

# Limit File Format

Whitespace separated

```
Mass
Unpol_0
Unpol_1
Opt_0
Opt_1
H20_0
H20_1
```

where

- `_0` → statistical only
- `_1` → statistical + systematic

---

# Cross Section File Format

Whitespace separated

```
Mass
CrossSection
YError
XError
```

---

# Supported Colliders

Currently implemented

- ILC
- CLIC
- FCC-ee

Each collider supports its own

- beam polarizations
- available mediator masses
- operating scenarios
- νν background cross section

through the `COLLIDER_INFO` configuration.

---

# Deployment

The dashboard can be deployed using

- Dash Enterprise
- Render
- Railway
- Heroku
- PythonAnywhere
- Docker
- Nginx + Gunicorn

A public deployment (if available) can be accessed at

```
<Deployment URL>
```

Replace this with the hosted URL after deployment.

---

# Adding New Colliders

Only the following need updating

- `COLLIDER_INFO`
- output data files

No changes are required in the plotting functions.

---

# Future Improvements

Planned features include

- Export figures as PDF
- Download selected histograms
- Cut-flow visualization
- Event yield tables
- Significance calculator
- Multi-scenario overlay
- Dark mode
- Automatic axis scaling

---

# Citation

If this dashboard contributes to your research, please cite the associated publication.

(Add DOI / arXiv here.)

---

# Author

**Saumyen Kundu**

Harish-Chandra Research Institute

Email: <your email>

GitHub: <GitHub profile>

---

# License

Specify the preferred license, for example

MIT License

or

GNU GPL v3

