"""
=========================================================
root_runner.py

Runs CERN ROOT macros from Python

Author : Saumyen Kundu

=========================================================
"""

import subprocess
import os


# -------------------------------------------------------
# Configuration
# -------------------------------------------------------

# Directory containing the Delphes installation
DELPHES_DIR = "/home/saumyen/HEP_Packages/Delphes"

ROOT_DIR = "./analysis"

MONOPH_MACRO = "MonoPh.C"
LIMIT_MACRO = "ContourPhIntf.C"

HISTOGRAM_FILE = "./output/histograms.root"
LIMIT_FILE = "./output/ContourIntW30M.dat"


# -------------------------------------------------------
# Execute a ROOT macro
# -------------------------------------------------------

def run_macro(macro, arguments):

    macro_path = os.path.join(ROOT_DIR, macro)

    if arguments == "":
        command = [
            "root",
            "-l",
            "-b",
            "-q",
            macro_path
        ]
    else:
        command = [
            "root",
            "-l",
            "-b",
            "-q",
            f'{macro_path}({arguments})'
        ]

    print("\nRunning ROOT macro")
    print("------------------")
    print(" ".join(command))
    print()

    process = subprocess.run(

        command,

        cwd=DELPHES_DIR,
    
        stdout=subprocess.PIPE,

        stderr=subprocess.PIPE,

        text=True

    )

    if process.returncode != 0:

        raise RuntimeError(

            "\nROOT ERROR\n\n"
            + process.stderr

        )

    print(process.stdout)

    return process.stdout


# -------------------------------------------------------
# Produce Histograms
# -------------------------------------------------------

def run_histograms(pol):

    print("Producing histograms...")

    run_macro(MONOPH_MACRO, pol)

    if not os.path.exists(HISTOGRAM_FILE):

        raise FileNotFoundError(

            HISTOGRAM_FILE +
            " was not produced."

        )

    return HISTOGRAM_FILE


# -------------------------------------------------------
# Produce gZ' limits
# -------------------------------------------------------

def run_limits():

    print("Producing limits...")

    run_macro(LIMIT_MACRO)

    if not os.path.exists(LIMIT_FILE):

        raise FileNotFoundError(

            LIMIT_FILE +
            " was not produced."

        )

    return LIMIT_FILE


# -------------------------------------------------------
# Run Everything
# -------------------------------------------------------

def run_analysis(pol=0):

    print("="*60)
    print("Running Complete Analysis")
    print("="*60)

    histfile = run_histograms(pol)

    limitfile = run_limits()

    print("\nFinished successfully.\n")

    return {

        "histogram": histfile,

        "limits": limitfile

    }


# -------------------------------------------------------
# Test
# -------------------------------------------------------

if __name__ == "__main__":

    files = run_analysis(pol=0)

    print(files)
