"""
=========================================================

callbacks.py

Dash callbacks for the Monophoton Dashboard

=========================================================
"""

from dash import Input
from dash import Output
from dash import State
from dash.exceptions import PreventUpdate

from plots import (
    photon_energy_figure,
    limit_plot,
    empty_histogram,
    empty_limit,
    read_limit_file,
)

from root_runner import run_analysis


# =========================================================
# Register Callbacks
# =========================================================

def register_callbacks(app):

    # -----------------------------------------------------
    # Run Analysis Button
    # -----------------------------------------------------

    @app.callback(

        [

            Output("histogram", "figure"),

            Output("limit-plot", "figure"),

            Output("signal-events", "children"),

            Output("background-events", "children"),

            Output("significance", "children"),

            Output("coupling-limit", "children")

        ],

        Input("run-analysis", "n_clicks"),

        [

            State("collider", "value"),

            State("polarisation", "value"),

            State("mass", "value")

        ]

    )

    def update_dashboard(

            n_clicks,

            collider,

            pol,

            mass

    ):

        # ---------------------------------------------
        # Initial page
        # ---------------------------------------------

        if n_clicks is None:

            return (

                empty_histogram(),

                empty_limit(),

                "Signal : ---",

                "Background : ---",

                "Significance : ---",

                "gZ' : ---"

            )

        # ---------------------------------------------
        # Run ROOT
        # ---------------------------------------------

        try:

            output = run_analysis(pol=0)

        except Exception as e:

            print(e)

            return (

                empty_histogram(),

                empty_limit(),

                "ROOT ERROR",

                "",

                "",

                ""

            )

        # ---------------------------------------------
        # Build figures
        # ---------------------------------------------

        hist_fig = photon_energy_figure(

            output["histogram"]

        )

        limit_fig = limit_plot(

            output["limits"]

        )

        # ---------------------------------------------
        # Read numerical results
        # ---------------------------------------------

        df = read_limit_file(

            output["limits"]

        )

        # nearest mass point

        row = df.iloc[

            (df["Mass"] - mass).abs().argsort()[:1]

        ]

        g_limit = row["g"].values[0]

        # ---------------------------------------------
        # Dummy values
        #
        # Replace these with values from your ROOT macro
        # ---------------------------------------------

        signal_events = 350.2

        background_events = 10452.8

        significance = 3.26

        # ---------------------------------------------
        # Return everything
        # ---------------------------------------------

        return (

            hist_fig,

            limit_fig,

            f"Signal : {signal_events:.1f}",

            f"Background : {background_events:.1f}",

            f"Significance : {significance:.2f}",

            f"gZ' Limit : {g_limit:.5f}"

        )
