# import dash
# from dash import html

# app = dash.Dash(__name__)

# #app.layout = html.Div([html.H1('Welcome to my Dashboard'),
# #html.P('This is a paragraph.')])
# app.layout = html.Div([html.H1("World Happiness Dashboard"), html.P(["Data source: ", html.A("World Happiness Report", href="https://www.kaggle.com/datasets/unsdsn/world-happiness", target="_blank")])])

import dash_ag_grid as dag
from dash import Dash, html
import pandas as pd

app = Dash()

df = pd.read_csv(
    "https://raw.githubusercontent.com/plotly/datasets/master/ag-grid/olympic-winners.csv"
)

columnDefs = [
    {"field": "athlete", "filter": True},
    {"field": "country"},
    {"field": "sport"},
    {"field": "year"},
]

app.layout = html.Div(
    dag.AgGrid(
        id="theme-color-scheme",
        rowData=df.to_dict("records"),
        columnDefs=columnDefs,
        columnSize="sizeToFit",
        dashGridOptions={
            "theme": {"function": "myTheme(themeAlpine, agGrid)"}
        },
    )
)

if __name__ == '__main__':
    app.run(debug=True, port=8090)
