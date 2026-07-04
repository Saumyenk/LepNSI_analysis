"""
============================================================

layout.py

Stage-1 Monophoton Dashboard

============================================================
"""

from dash import html, dcc
import dash_bootstrap_components as dbc

def plot_toolbar(name, default_value):

    return dbc.Row(

        [

            dbc.Col(

                dbc.Checklist(

                    id=f"{name}-axis",

                    options=[

                        {"label": "Log X", "value": "logx"},

                        {"label": "Log Y", "value": "logy"},

                        {"label": "Grid", "value": "grid"}

                    ],

                    value=default_value,

                    inline=True,

                    switch=True,

                ),

                width=8,

            ),

            dbc.Col(

                dbc.ButtonGroup(

                    [

                        dbc.Button(
                            "PNG",
                            id=f"{name}-png",
                            color="secondary",
                            size="sm"
                        ),

                        dbc.Button(
                            "PDF",
                            id=f"{name}-pdf",
                            color="secondary",
                            size="sm"
                        ),

                    ]

                ),

                width=4,

                style={"textAlign": "right"}

            ),

        ],

        className="mb-2"

    )
    
def plot_tools_title(name, default_value):
    return dbc.Checklist(
        id=f"{name}-axis",
        options=[
            {"label":"Log X","value":"logx"},
            {"label":"Log Y","value":"logy"},
            {"label":"Grid","value":"grid"}
        ],
        className="toolbar",
        value=default_value,
        inline=True,
        switch=True
    )


def create_layout():

    return dbc.Container(

        [

        #######################################################
        # Header
        #######################################################

        dbc.Row(

            [dbc.Col(

                html.Div(

                    [

                    html.H2(
                        "Neutrino Leptonic NSI at e⁺e⁻ Colliders",
                        className="text-left mt-3",
                        style={"color": "orange"}#, "background": "steelblue"}
                    ),

                    html.H5(
                        # "e⁺e⁻ → γ + Invisible",
                        dcc.Markdown(r"$e^{-} e^{+} \to \gamma + E\llap{/}_{T}$", mathjax=True),
                        className="text-left",
                        style={"color": "lightgreen"}
                    ),

                    # html.Hr(),
                    # html.Br(),
                    html.Div(style={"height": "0.6em"})  # Acts as a half-sized line break


                    ]

                ),

                width=9

            ),

            dbc.Col(
                [
                    html.Img(
                        src="/assets/HRI_logo.png",
                        style={
                            "height": "90px",
                            "float": "right",
                            "marginTop": "8px",
                            "marginRight": "10px"
                        }
                    )
                ],
                width=3,
                className="d-flex justify-content-end align-items-center"
            ),],

            className="mb-3",

            style={"backgroundColor": "#0B3C5D", "color": "white"}
            # style={"backgroundColor": "rgba(50, 120, 150, 1)", "color": "white"}


        ),

        # html.Br(),

        #######################################################
        # Controls
        #######################################################

        dbc.Card(

            dbc.CardBody(

                dbc.Row(

                    [

                    dbc.Col(

                        [

                        html.Label("Collider"),

                        dcc.Dropdown(

                            id="collider",

                            options=[

                                {"label":"ILC", "value":"ILC"},
                                {"label":"CLIC", "value":"CLIC"},
                                {"label":"FCC", "value":"FCC"}

                            ],

                            value="ILC",

                            clearable=False

                        )

                        ],

                        md=3

                    ),

                    dbc.Col(md=3),

                    dbc.Col(

                        [

                        html.Label("Observable"),

                        dcc.Dropdown(

                            id="observable",

                            options=[

                                {
                                    "label":"Photon Energy",
                                    "value":"PhotonE"
                                },

                                {
                                    "label":"cosθ",
                                    "value":"CosTheta"
                                }

                            ],

                            value="PhotonE",

                            clearable=False

                        )

                        ],

                        md=3

                    ),

                    dbc.Col(

                        [

                        html.Label("Mass (GeV)"),

                        dcc.Dropdown(

                            id="mass",

                            options=[

                                {"label":"10","value":10},
                                {"label":"50","value":50},
                                {"label":"100","value":100},
                                {"label":"200","value":200},
                                {"label":"500","value":500},
                                {"label":"1000","value":1000},
                                {"label":"2000","value":2000},
                                {"label":"5000","value":5000},
                                {"label":"10000","value":10000}

                            ],

                            value=500,

                            clearable=False

                        )

                        ],

                        md=3

                    )

                    ]

                )

            ),

            className="mb-3",

            # style={"backgroundColor": "rgba(50, 130, 180, 0.95)", "color": "white"}

        ),

        #######################################################
        # X-sec and Histogram
        #######################################################

        dbc.Row(

            [

                dbc.Col(

                    dbc.Card(

                        [

                            dbc.CardHeader(

                                dbc.Row(
                                    [
                                        dbc.Col(
                                            html.H6("Cross Section"),
                                            width=4
                                        ),

                                        dbc.Col(
                                            plot_tools_title("xsec",["logx","grid"]),
                                            width=8,
                                            style={"textAlign": "right"}
                                        )
                                    ],
                                    align="center"
                                )

                            ),

                            dbc.CardBody(

                                # [plot_toolbar("xsec",["logx","grid"]),

                                dcc.Graph(

                                    id="xsec-plot",
                                    
                                    mathjax=True,

                                    config={

#                                        "displaylogo": False,
                                
#                                        "displayModeBar": False,
                                        
                                        "responsive": True

                                    },
                                    
                                    style={"height": "100%",
                                           "width": "100%"},

                                ),#],
                                
                                style={"height": "570px"}

                            )

                        ]

                    ),

                    md=6

                ),

                dbc.Col(

                    dbc.Card(

                        [

                            dbc.CardHeader(

                                dbc.Row(
                                    [
                                        dbc.Col(
                                            html.H6("Histogram"),
                                            width=4
                                        ),

                                        dbc.Col(
                                            plot_tools_title("hist", ["logy", "grid"]),
                                            width=8,
                                            style={"textAlign": "right", "color": "white"}
                                        )
                                    ],
                                    align="center"
                                )

                            ),

                            dbc.CardBody(

                                # [plot_toolbar("hist",["logy","grid"]),
                                
                                dcc.Graph(

                                    id="histogram",
                                    
                                    mathjax=True,

                                    config={

#                                        "displaylogo": False,
                                
#                                        "displayModeBar": False,
                                        
                                        "responsive": True

                                    },
                                    
                                    style={"height": "100%",
                                           "width": "100%"},

                                ),#],
                                
                                style={"height": "570px"}

                            )

                        ]

                    ),

                    md=6

                )

            ],

            className="mb-4"

        ),

        #######################################################
        # Limit plot and Summary
        #######################################################

        dbc.Row(

            [

                dbc.Col(

                    dbc.Card(

                        [

                            # dbc.CardHeader("NSI Sensitivity"),
                            dbc.CardHeader(

                                dbc.Row(
                                    [
                                        dbc.Col(
                                            html.H6("NSI Sensitivity"),
                                            width=4
                                        ),

                                        dbc.Col(
                                            plot_tools_title("limit",["logx","logy","grid"]),
                                            width=8,
                                            style={"textAlign": "right"}
                                        )
                                    ],
                                    align="center"
                                )

                            ),


                            dbc.CardBody(

                                # [plot_toolbar("limit",["logx","logy","grid"]),

                                dcc.Graph(

                                    id="limit-plot",
                                    
                                    mathjax=True,

                                    config={
#                                        "displaylogo": False,
#                                        "displayModeBar": False,
                                        "responsive": True
                                    },
                                    
                                    style={"height": "100%",
                                           "width": "100%"},

                                ),#],
                                
                                style={"height": "570px"}

                            )

                        ]

                    ),

                    md=6

                ),

                dbc.Col(

                    dbc.Card(

                        [

                            # dbc.CardHeader("Analysis Summary"),
                            dbc.CardHeader(

                                dbc.Row(
                                    [
                                        dbc.Col(
                                            html.H6("Analysis Summary"),
                                            width=4
                                        )
                                    ],
                                    align="center"
                                )

                            ),

                            dbc.CardBody(

                                [

#                                    dbc.Alert(
#                                        id="signal-events",
#                                        children="Signal : ---",
#                                        color="primary"
#                                    ),

#                                    dbc.Alert(
#                                       id="background-events",
#                                        children="Background : ---",
#                                        color="warning"
#                                    ),

#                                    dbc.Alert(
#                                         id="significance",
#                                        children="Significance : ---",
#                                        color="success"
#                                    ),

                                    dcc.Markdown(
                                        id="coupling-limit",
#                                        children="gZ' : ---",
                                        mathjax=True,
                                        style={
                                            "color": "#721c24",          # Dark red text (Bootstrap alert-danger text color)
                                            "backgroundColor": "#f8d7ac",  # Soft, muted pastel red background
                                            "border": "1px solid #f5c6cb", # Matching subtle border tint
                                            "padding": "12px 18px",      # Internal spacing around text
                                            "borderRadius": "7px",       # Clean, rounded Bootstrap corners
                                            "fontWeight": "bold",        # Keeps your original font weight
                                            "fontSize": "18px",          # Keeps your original font size
                                            "margin": "0 0",          # Optional: creates breathing room outside the box
                                            "marginBottom": "0px",   # <--- REMOVES EXTRA SPACE BELOW THE BOX
                                            "paddingBottom": "2px",  # Balanced internal padding                                            
                                            "display": "block"           # Ensures it behaves as a full-width container block
                                        }
                                    ),

                                ]

                            )

                        ]

                    ),

                    md=6

                )

            ]

        ),
        
        #######################################################
        # Summary
        #######################################################
#        html.Footer("© 2026 Your Company Name. All Rights Reserved.",
        html.Footer("by Saumyen Kundu",
            style={
                "textAlign": "left",
                "fontSize": "14px",
                "padding": "2.5px",
                "paddingLeft": "5pt",
                "borderRadius": "4px",
                "background": "lightgray",
                "color": "darkgreen",
                "marginTop": "auto" # Pushes footer to bottom if page content is short
            },
            className="mt-3"
        )
        ],

        fluid=True

    )
