"""
========================================================

plots.py

Functions for producing Plotly figures
from ROOT histograms and limit files.

========================================================
"""

import numpy as np
import pandas as pd
import uproot

import plotly.graph_objects as go


#-------------------------------------------------------
# ROOT Histogram
#-------------------------------------------------------

def load_histogram(rootfile,
                   histname="E_{photon}_1"):

    """
    Reads a TH1 histogram from a ROOT file
    """

    file = uproot.open(rootfile)

    hist = file["Signal/PhotonE"]

    values = hist.values()

    edges = hist.axis().edges()

    centres = 0.5*(edges[:-1]+edges[1:])

    return centres, values


#-------------------------------------------------------
# Photon Energy Figure
#-------------------------------------------------------

def photon_energy_figure(rootfile):

    x, y = load_histogram(rootfile)

    fig = go.Figure()

    fig.add_trace(

        go.Scatter(

            x=x,

            y=y,

            mode="lines",

            line=dict(
                color="royalblue",
                width=3
            ),

            fill="tozeroy",

            name="Photon"

        )

    )

    fig.update_layout(

        template="plotly_white",

        title="Photon Energy Distribution",

        xaxis_title="Photon Energy (GeV)",

        yaxis_title="1/N dN/dE",

        font=dict(size=16),

        height=500,

        legend=dict(

            x=0.75,
            y=0.95

        )

    )

    return fig


#-------------------------------------------------------
# Read gZ' limit file
#-------------------------------------------------------

def read_limit_file(filename):

    """
    Assumes the ROOT macro writes

    Mass    gLimit

    or

    Mass  Significance  gLimit

    Modify columns if needed.
    """

    df = pd.read_csv(

        filename,

        delim_whitespace=True,

        comment="#",

        header=None

    )

    if len(df.columns)==2:

        df.columns=[

            "Mass",

            "g"

        ]

    elif len(df.columns)>=3:

        df.columns=[

            "Mass",

            "Sig",

            "g"

        ]

    return df


#-------------------------------------------------------
# gZ' Figure
#-------------------------------------------------------

def limit_plot(filename):

    df = read_limit_file(filename)

    fig = go.Figure()

    fig.add_trace(

        go.Scatter(

            x=df["Mass"],

            y=df["g"],

            mode="lines+markers",

            line=dict(

                color="firebrick",

                width=3

            ),

            marker=dict(

                size=8

            ),

            name="ILC"

        )

    )

    fig.update_layout(

        template="plotly_white",

        title=r"$g_{Z'}$ Sensitivity",

        xaxis=dict(

            title="M(Z') [GeV]",

            type="log"

        ),

        yaxis=dict(

            title="gZ'",

            type="log"

        ),

        height=500,

        font=dict(size=16)

    )

    return fig


#-------------------------------------------------------
# Empty histogram
#-------------------------------------------------------

def empty_histogram():

    fig=go.Figure()

    fig.update_layout(

        template="plotly_white",

        title="Photon Energy Distribution",

        xaxis_title="Photon Energy (GeV)",

        yaxis_title="Entries",

        annotations=[

            dict(

                text="Run Analysis",

                showarrow=False,

                font=dict(

                    size=22,

                    color="gray"

                ),

                x=0.5,

                y=0.5,

                xref="paper",

                yref="paper"

            )

        ]

    )

    return fig


#-------------------------------------------------------
# Empty limit plot
#-------------------------------------------------------

def empty_limit():

    fig=go.Figure()

    fig.update_layout(

        template="plotly_white",

        title=r"$g_{Z'}$ Sensitivity",

        xaxis=dict(

            title="Mass",

            type="log"

        ),

        yaxis=dict(

            title="g",

            type="log"

        ),

        annotations=[

            dict(

                text="Run Analysis",

                showarrow=False,

                font=dict(

                    size=22,

                    color="gray"

                ),

                x=0.5,

                y=0.5,

                xref="paper",

                yref="paper"

            )

        ]

    )

    return fig
