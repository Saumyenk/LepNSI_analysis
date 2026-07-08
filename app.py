"""
===========================================================
 Monophoton Analysis Dashboard
 Stage-1
 Author : Saumyen Kundu
===========================================================
"""

from dash import Dash
import dash_bootstrap_components as dbc

from src.layout import create_layout
from src.callbacks import register_callbacks



# ----------------------------------------------------
# Create App
# ----------------------------------------------------

app = Dash(
    __name__,
    external_stylesheets=[
        dbc.themes.MORPH
    ],
    suppress_callback_exceptions=True
)

# server = app.server

# ----------------------------------------------------
# Layout
# ----------------------------------------------------

app.layout = create_layout()

# ----------------------------------------------------
# Callbacks
# ----------------------------------------------------

register_callbacks(app)

# ----------------------------------------------------
# Run Server
# ----------------------------------------------------

if __name__ == "__main__":

    app.run(
#        host="0.0.0.0",
        port=8050,
#        debug=False,
#        dev_tools_hot_reload=False    
        debug=True
        )
