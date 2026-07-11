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
# Collider Info
# ------------------------------------------------------------
COLLIDER_INFO = {

    "ILC": {

        "polarisations": [
            ("Unpolarized", 0),
            ("(-80,+30)", 1),
            ("(+80,-30)", 2),
            ("(-80,-30)", 3),
            ("(+80,+30)", 4),
        ],

        "masses": [1,100,500,950,1000,1050,1500,2000,5000,10000],

        "nunu_xsec": [5411.5189, 1494.2042, 946.57917, 12533.085, 6759.6076],

        "limit_scenarios": [
            ("Unpol", "Unpol"),
            ("Opt. Pol", "Opt"),
            ("H20", "H20"),
        ]

    },

    "CLIC": {

        "polarisations": [
            ("Unpolarized",0),
            ("(-80,+30)",1),
            ("(+80,-30)",2),
        ],

        "masses": [1,500,1000,2000,2500,2950,3000,3050,5000,10000],

        "nunu_xsec": [7722.2237, 1218.7375, 10554.297],

        "limit_scenarios": [
            ("Unpol", "Unpol"),
            ("Opt. Pol", "Opt"),
            ("Mix. Pol", "H20"),
        ]

    },

    "FCC": {

        "polarisations": [
            ("Unpol",0),
        ],

        "masses": [1, 100, 300, 360, 365, 370, 500, 1000, 5000, 10000],

        "nunu_xsec": [5762.3081],

        "limit_scenarios": [
            ("Unpolarized", "Unpol"),
        ]

    }

}

# ------------------------------------------------------------
# Files
# ------------------------------------------------------------
def get_files(collider, width, pol):
    base = f"./output/{collider}"
    return {
        "root": f"{base}/Histo/histo_{width}_{pol}.root",
        "limit1": f"{base}/Limit/Pure_{width}.dat",
        "limit2": f"{base}/Limit/Int_{width}.dat",
        "CSFILE1": f"{base}/Xsec/CS_Pure_{width}_{pol}.txt", # Pure
        "CSFILE2": f"{base}/Xsec/CS_Full_{width}_{pol}.txt", # Interference
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


    @app.callback(
        Output("polarization","options"),
        Output("polarization","value"),

        Output("mass","options"),
        Output("mass","value"),

        Output("limit-scenarios","options"),
        Output("limit-scenarios","value"),


        # Output("vv_xsec","value"),

        Input("collider","value")

    )
    def update_collider_options(collider):
        info = COLLIDER_INFO[collider]
        pol_options = [
            {
                "label": label,
                "value": value
            }
            for label,value in info["polarisations"]
        ]

        mass_options = [
            {
                "label": str(m),
                "value": m
            }
            for m in info["masses"]
        ]

        scenario_options = [
            {
                "label": label,
                "value": value
            }
            for label,value in info["limit_scenarios"]
        ]

        return (
            pol_options,
            pol_options[0]["value"],
            mass_options,
            mass_options[0]["value"],

            scenario_options,
            [value for _, value in info["limit_scenarios"]]
        )


    # def update_limit_scenarios(collider):
    #     scenarios = COLLIDER_INFO[collider]["limit_scenarios"]
    #     options = [
    #         {"label": label, "value": value}
    #         for label, value in scenarios
    #     ]
    #     values = [value for _, value in scenarios]
    #     return options, values
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
            Input("polarization", "value"),
            Input("observable", "value"),
            Input("mass", "value"),
            Input("width","value"),

            Input("limit-scenarios","value"),
            Input("limit-systematics","value"),

            Input("xsec-axis","value"),
            Input("hist-axis","value"),
            Input("limit-axis","value")
        ]
    )

    def update_dashboard(collider,
                         polarization,
                         observable,
                         mass,
                         width,
                         limit_scenarios,
                         limit_systematics,
                         xsec_axis,
                         hist_axis,
                         limit_axis):
        
        info = COLLIDER_INFO[collider]

        if polarization is None:
            polarization = 0

        if mass is None:
            mass = info["masses"][0]

        files = get_files(collider, width, polarization)
        vv_xsec = info["nunu_xsec"][polarization]

        # Decode toolbar options
        xsec_opts = decode_axis_options(xsec_axis)
        hist_opts = decode_axis_options(hist_axis)
        limit_opts = decode_axis_options(limit_axis)
        
        # --------------------------------------------
        # Histogram
        # --------------------------------------------

        hist_fig = histogram_figure(
            files["root"],
            mass,
            observable,
            **hist_opts
        )

        # --------------------------------------------
        # Limit plot
        # --------------------------------------------

        limit_fig = limit_plot(
            files["limit1"],
            files["limit2"],
            # scenario="H20_1",
            scenarios=limit_scenarios,
            systematic=limit_systematics,
            **limit_opts
        )

        # --------------------------------------------
        # Xsection plot
        # --------------------------------------------

        xsec_fig = xsec_plot(
            files["CSFILE1"], 
            files["CSFILE2"],
            vv_xsec,
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
