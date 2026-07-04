"""
============================================================

callbacks.py

Callbacks for the Monophoton Dashboard

============================================================
"""

from dash import Input, Output

from src.plots import (
    histogram_figure,
    limit_plot,
    xsec_plot,
    get_summary,
)

# ------------------------------------------------------------
# Files
# ------------------------------------------------------------
def get_files(collider):
    base = f"./output/{collider}"
    return {
        "root": f"{base}/histograms.root",
        "limit1": f"/home/saumyen/Research/NSI_Coll/{collider}/ContourDataPol.dat",
        "limit2": f"{base}/ContourIntW30M_Jun26.dat",
        "CSFILE1": f"/home/saumyen/Research/NSI_Coll/{collider}/Signal/W10M/Pol0/CS_M2.txt", # Pure
        "CSFILE2": f"/home/saumyen/Research/NSI_Coll/Interference2/{collider}/Signal10/Pol0/CS_M0.txt", # Interference
    }

#ROOTFILE = "./output/histograms.root"

#LIMITFILE = "./output/ContourIntW30M_Jun26.dat"

#CSFILE1 = "/home/saumyen/Research/NSI_Coll/ILC/Signal/W10M/Pol0/CS_M2.txt"
#CSFILE2 = "/home/saumyen/Research/NSI_Coll/Interference2/ILC/Signal10/Pol0/CS_M0.txt"

def decode_axis_options(options):
    options = options or []
    return {
        "logx": "logx" in options,
        "logy": "logy" in options,
        "grid": "grid" in options,
    }
# ------------------------------------------------------------
# Register callbacks
# ------------------------------------------------------------

def register_callbacks(app):

    # --------------------------------------------------------
    # Update histogram, limit plot and summary
    # --------------------------------------------------------

    @app.callback(

        [
            Output("histogram", "figure"),
            Output("limit-plot", "figure"),
            Output("xsec-plot", "figure"),
#            Output("signal-events", "children"),
#            Output("background-events", "children"),
#            Output("significance", "children"),
            Output("coupling-limit", "children"),
        ],
        [

#            Input("process", "value"),
            Input("collider", "value"),
            Input("observable", "value"),
            Input("mass", "value"),

            Input("xsec-axis","value"),
            Input("hist-axis","value"),
            Input("limit-axis","value")
        ]
    )

    def update_dashboard(collider,
                         observable,
                         mass,
                         xsec_axis,
                         hist_axis,
                         limit_axis):
        
        files = get_files(collider)

        # Decode toolbar options
        xsec_opts = decode_axis_options(xsec_axis)
        hist_opts = decode_axis_options(hist_axis)
        limit_opts = decode_axis_options(limit_axis)
        

        # --------------------------------------------
        # Histogram
        # --------------------------------------------

        hist_fig = histogram_figure(
            files["root"],
            observable,
            **hist_opts
        )

        # --------------------------------------------
        # Limit plot
        # --------------------------------------------

        limit_fig = limit_plot(
            files["limit1"],
            files["limit2"],
            scenario="H20_1",
            **limit_opts
        )

        # --------------------------------------------
        # Xsection plot
        # --------------------------------------------

        xsec_fig = xsec_plot(
            files["CSFILE1"], files["CSFILE2"],
            scenario="xsec",
            **xsec_opts
        )

        # --------------------------------------------
        # Read g limit
        # --------------------------------------------

        summary = get_summary(
            files["limit1"],
            mass
        )

        g_limit = summary["g"]

        # --------------------------------------------
        # Placeholder values
        #
        # Replace these later by reading from ROOT
        # --------------------------------------------

        signal = "---"
        background = "---"
        significance = "---"

        # --------------------------------------------
        # Return
        # --------------------------------------------

        return (

            hist_fig,
            limit_fig,
            xsec_fig,

#            f"Signal : {signal}",
#            f"Background : {background}",
#            f"Significance : {significance}",

            r"$\mathbf{\epsilon_{\alpha\beta}^{ee}} " + f": {g_limit:.5g}$"
        )
