"""
=========================================================
layout.py

Creates the GUI layout for the Monophoton Dashboard

=========================================================
"""

from dash import html, dcc
import dash_bootstrap_components as dbc


def create_layout():

    return dbc.Container(

        [

        # =====================================================
        # Title
        # =====================================================

        dbc.Row(

            dbc.Col(

                html.H2(
                    "Monophoton Analysis Dashboard",
                    className="text-center text-primary mt-3 mb-3"
                )

            )

        ),

        dbc.Row(

            dbc.Col(

                html.H5(
                    r"$e^+e^- \rightarrow \gamma + \mathrm{Invisible}$",
                    className="text-center mb-4"
                )

            )

        ),

        html.Hr(),


        # =====================================================
        # Controls
        # =====================================================

        dbc.Card(

            dbc.CardBody(

                [

                dbc.Row(

                    [

                    #################################################

                    dbc.Col(

                        [

                        html.Label("Collider"),

                        dcc.Dropdown(

                            id="collider",

                            options=[

                                {
                                    "label":"ILC (1 TeV)",
                                    "value":"ILC"
                                }

                            ],

                            value="ILC",

                            clearable=False

                        )

                        ],

                        width=3

                    ),

                    #################################################

                    dbc.Col(

                        [

                        html.Label("Polarisation"),

                        dcc.Dropdown(

                            id="polarisation",

                            options=[

                                {"label":"Pol-0","value":1},
                                {"label":"Pol-1","value":1},
                                {"label":"Pol-2","value":2},
                                {"label":"Pol-3","value":3},
                                {"label":"Pol-4","value":4}

                            ],

                            value=1,

                            clearable=False

                        )

                        ],

                        width=3

                    ),

                    #################################################

                    dbc.Col(

                        [

                        html.Label("Z' Mass (GeV)"),

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
                                {"label":"5000","value":5000}

                            ],

                            value=1000,

                            clearable=False

                        )

                        ],

                        width=3

                    ),

                    #################################################

                    dbc.Col(

                        [

                        html.Br(),

                        dbc.Button(

                            "Run ROOT Analysis",

                            id="run-analysis",

                            color="success",

                            size="lg",

                            className="w-100"

                        )

                        ],

                        width=3

                    )

                    ]

                )

                ]

            ),

            className="mb-4"

        ),


        # =====================================================
        # Histogram
        # =====================================================

        dbc.Card(

            [

            dbc.CardHeader(

                html.H4("Photon Energy Distribution")

            ),

            dbc.CardBody(

                [

                dcc.Loading(

                    dcc.Graph(

                        id="histogram",

                        style={"height":"500px"}

                    ),

                    type="circle"

                )

                ]

            )

            ],

            className="mb-4"

        ),


        # =====================================================
        # gZ' Plot
        # =====================================================

        dbc.Card(

            [

            dbc.CardHeader(

                html.H4(r"$g_{Z'}$ Sensitivity")

            ),

            dbc.CardBody(

                [

                dcc.Loading(

                    dcc.Graph(

                        id="limit-plot",

                        style={"height":"500px"}

                    ),

                    type="circle"

                )

                ]

            )

            ],

            className="mb-4"

        ),


        # =====================================================
        # Summary
        # =====================================================

        dbc.Card(

            [

            dbc.CardHeader(

                html.H4("Analysis Summary")

            ),

            dbc.CardBody(

                [

                dbc.Row(

                    [

                    dbc.Col(

                        dbc.Alert(

                            id="signal-events",

                            children="Signal : ---",

                            color="primary"

                        )

                    ),

                    dbc.Col(

                        dbc.Alert(

                            id="background-events",

                            children="Background : ---",

                            color="warning"

                        )

                    ),

                    dbc.Col(

                        dbc.Alert(

                            id="significance",

                            children="Significance : ---",

                            color="success"

                        )

                    ),

                    dbc.Col(

                        dbc.Alert(

                            id="coupling-limit",

                            children=r"g_Z' : ---",

                            color="danger"

                        )

                    )

                    ]

                )

                ]

            )

            ],      # <-- THIS IS MISSING IN YOUR FILE

            className="mb-4"
        ),

        html.Br(),

        ],

        fluid=True

    )
