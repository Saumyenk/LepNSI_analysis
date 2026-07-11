"""
============================================================

plots.py

Reads ROOT histograms and limit file for the
Monophoton Dashboard.

============================================================
"""

import numpy as np
import pandas as pd
import uproot

import plotly.graph_objects as go

GF = 1.1663787e-5; # in Gev^2 

# ------------------------------------------------------------
# Read Histogram
# ------------------------------------------------------------

def load_histogram(rootfile, process, observable):
    """
    Read a TH1 histogram from

    Signal/PhotonE
    NuNu/PhotonE
    Bhabha/PhotonE
    Combined/PhotonE
    """

    with uproot.open(rootfile) as f:
        
        hist = f[f"{process}/{observable}"]
        
        values = hist.values()
        
        edges = hist.axis().edges()
    
    centres = 0.5 * (edges[:-1] + edges[1:])

    return centres, values


# ------------------------------------------------------------
# Histogram Figure
# ------------------------------------------------------------

def histogram_figure(rootfile, mass, observable,
                     logx=False, logy=True, grid=True):

    processes = [
        (f"Signal/M{mass}", "red"),
        ("NuNu", "royalblue"),
        ("Bhabha", "forestgreen"),
        (f"Full/M{mass}", "black")
    ]

    titles = {
        "PhotonE": r"$E_{\gamma}\;\rm{ [GeV]}$",
        "CosTheta": r"cos($\theta_\gamma$)"
    }

    fig = go.Figure()

    with uproot.open(rootfile) as f:

        for process, colour in processes:

            hist = f[f"{process}/{observable}"]

            values = hist.values()

            edges = hist.axis().edges()

            centres = 0.5*(edges[:-1] + edges[1:])

            fig.add_trace(

                go.Scatter(

                    x=centres,

                    y=values,

                    mode="lines",

                    line=dict(
                        color=colour,
                        width=2,
                        shape="hv"
                    ),

                    name=process,

                    hovertemplate=(
                        "<b>%{fullData.name}</b><br>"
                        "Bin centre = %{x:.2f}<br>"
                        "Norm events = %{y:.2e}<extra></extra>"
                    )

                )

            )

    fig.update_layout(

        template="plotly_white",

        height=500,
        
        margin=dict(
            l=70,
            r=40,
            t=5,
            b=5
        ),

        autosize=True,


#        title=titles.get(observable, observable),

        xaxis_title=titles.get(observable, observable),

        yaxis_title="Normalized Events",

        hovermode="x unified",

        legend=dict(
            x=0.75,
            y=0.98,
            bgcolor="rgba(255,255,255,0.7)", font_size=19
        ),

        xaxis=dict(
            showgrid=True,
            mirror=True,
            showline=True,
            type="log" if logx else "linear",
            linewidth=2
        ),

        yaxis=dict(
            showgrid=True,
            mirror=True,
            showline=True,
            linewidth=2,
            type="log" if logy else "linear",
            exponentformat="power"
        )

    )

    return fig

# ------------------------------------------------------------
# Read limit file
# ------------------------------------------------------------

def read_limit_file(filename):

    df = pd.read_csv(

        filename,

        sep=r"\s+",

        comment="#",

        header=None,

        names=[

            "Mass",
            "Unpol_0",
            "Unpol_1",
            "Opt_0",
            "Opt_1",
            "H20_0",
            "H20_1"

        ]

    )

    return df
# ------------------------------------------------------------
# Limit Plot
# ------------------------------------------------------------

def limit_plot(filename1, filename2, scenarios=["Unpol","Opt","H20"], systematic=True,
               logx=True, logy=True, grid=True):

    df1 = read_limit_file(filename1)
    df2 = read_limit_file(filename2)
    # df1 = df1.dropna(subset=[scenario]) # removes NaNs
    # df2 = df2.dropna(subset=[scenario]) # removes NaNs

    columns = {
        "Unpol": {
            True: "Unpol_1",
            False: "Unpol_0"
        },
        "Opt": {
            True: "Opt_1",
            False: "Opt_0"
        },
        "H20": {
            True: "H20_1",
            False: "H20_0"
        }
    }


    fig = go.Figure()

    for scenario in scenarios:

        column = columns[scenario][systematic]
        df1 = df1.dropna(subset=[column]) # removes NaNs
        df2 = df2.dropna(subset=[column]) # removes NaNs


        fig.add_trace(

            go.Scatter(

                x=df1["Mass"],

                y=df1[column]/(2*np.sqrt(2)*GF*df1["Mass"]*df1["Mass"]),

                mode="lines+markers",

                # name=f"ILC ({scenario})",
                name=f"w/o Intf",

                line=dict(width=3),

                marker=dict(size=7)

            )

        )
        fig.add_trace(

            go.Scatter(

                x=df2["Mass"],

                y=df2[column]/(2*np.sqrt(2)*GF*df2["Mass"]*df2["Mass"]),

                mode="lines+markers",

                # name=f"ILC ({scenario})",
                name=f"w/ Intf",

                line=dict(width=3),

                marker=dict(size=7)

            )

        )



    fig.update_layout(

        template="plotly_white",

#        title="3σ Coupling Sensitivity",

        height=500,

        margin=dict(
            l=70,
            r=40,
            t=5,
            b=5
        ),

        autosize=True,

        legend=dict(
            x=0.85,
            y=0.98,
            bgcolor="rgba(255,255,255,0.7)", font_size=19
        ),

        xaxis=dict(

            title=r"$M_{Z\prime}\;\rm{ [GeV]}$",

            type="log" if logx else "linear",

            showline=True

        ),

        yaxis=dict(

            title=r"$|\epsilon|$",

            type="log" if logy else "linear",

            showline=True

        )

    )

    return fig
# ------------------------------------------------------------
# Read xsec file
# ------------------------------------------------------------

def read_xsec_file(filename):

    df = pd.read_csv(

        filename,

        sep=r"\s+",

        comment="#",

        header=None,

        names=[

            "mass",
            "xsec",
            "yerr",
            "xerr"

        ]

    )

    return df
# ------------------------------------------------------------
# Xsection Plot
# ------------------------------------------------------------

def xsec_plot(filename1, filename2, nunu_xsec, scenario="xsec",
              logx=True, logy=False, grid=True):

    df1 = read_xsec_file(filename1)
    df2 = read_xsec_file(filename2)

    fig = go.Figure()

# w/o interference
    fig.add_trace(

        go.Scatter(

            x=df1["mass"],

            y=df1[scenario],

            mode="lines+markers",

            name=f"w/o Intf",

            line=dict(width=3),

            marker=dict(size=7)

        )

    )
# w/o interference
    fig.add_trace(

        go.Scatter(

            x=df2["mass"],

            y=df2[scenario]-nunu_xsec,

            mode="lines+markers",

            name=f"w/  Intf",

            line=dict(width=3),

            marker=dict(size=7)

        )

    )

    fig.update_layout(

        template="plotly_white",

#        title="Cross-section Plot",

        height=500,

        margin=dict(
            l=20,
            r=10,
            t=5,
            b=5
        ),

        autosize=True,

        legend=dict(
            x=0.80,
            y=0.90,
            bgcolor="rgba(255,255,255,0.7)", font_size=19
        ),

        xaxis=dict(

            title=r"$M_{Z\prime}\;\rm{ [GeV]}$",

            type="log" if logx else "linear",

            showline=True

        ),

        yaxis=dict(

            title=r"$\sigma(e^-e^+\to\gamma\nu\bar{\nu})\;\rm{[fb]}$",

            type="log" if logy else "linear",

            showline=True

        )

    )

    return fig
# ------------------------------------------------------------
# Summary
# ------------------------------------------------------------

def get_summary(limitfile, mass):

    df = read_limit_file(limitfile)

    idx = np.argmin(np.abs(df["Mass"]-mass))

    row = df.iloc[idx]

    return {

        "g" : row["H20_1"]

    }
