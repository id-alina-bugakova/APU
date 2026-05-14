import plotly.graph_objects as go
import dash
from dash import dcc, html, Input, Output, State, callback
import glob
import os
import numpy as np


class PlotlyViewer:
    def __init__(self, file_pattern="*.data"):
        self.file_pattern = file_pattern
        self.files = []
        self.app = None

    def load_data(self, filename):
        """Загрузка данных из файла"""
        numbers = []
        try:
            with open(filename, 'r') as file:
                for line in file:
                    line = line.strip()
                    if line:
                        numbers.append(float(line))
        except Exception as e:
            print(f"Ошибка загрузки {filename}: {e}")
            numbers = []
        return numbers

    def create_figure(self, data, filename, show_stats=True):
        """Создание графика со статистикой"""
        fig = go.Figure()

        # Основной график
        fig.add_trace(go.Scatter(
            y=data,
            mode='lines+markers',
            name='Данные',
            marker=dict(size=6, color='royalblue'),
            line=dict(width=2, color='royalblue'),
            hovertemplate='Индекс: %{x}<br>Значение: %{y:.3f}<extra></extra>'
        ))

        fig.update_layout(
            title=f"<b>{filename}</b>",
            xaxis_title="<b>Индекс</b>",
            yaxis_title="<b>Значение</b>",
            template="plotly_white",
            hovermode='x unified',
            height=600,
            margin=dict(l=50, r=50, t=80, b=50)
        )

        fig.update_xaxes(showgrid=True, gridwidth=1, gridcolor='lightgray')
        fig.update_yaxes(showgrid=True, gridwidth=1, gridcolor='lightgray')

        return fig

    def create_app(self):
        """Создание Dash приложения"""
        self.files = sorted(glob.glob(self.file_pattern))

        if not self.files:
            print(f"Файлы по шаблону '{self.file_pattern}' не найдены!")
            return None

        self.app = dash.Dash(__name__)

        # Создаем вкладки
        tabs = []
        for i, filename in enumerate(self.files):
            data = self.load_data(filename)
            if not data:
                continue

            fig = self.create_figure(data, os.path.basename(filename))

            tabs.append(
                dcc.Tab(
                    label=f"{os.path.basename(filename)[:20]}",
                    children=[
                        html.Div([
                            dcc.Graph(
                                id=f'graph-{i}',
                                figure=fig,
                                config={'displayModeBar': True}
                            ),
                            html.Div([
                                html.Button(
                                    f"📊 Экспорт графика",
                                    id=f'export-btn-{i}',
                                    style={'margin': '5px'}
                                ),
                                html.Button(
                                    f"🔄 Обновить",
                                    id=f'refresh-btn-{i}',
                                    style={'margin': '5px'}
                                ),
                            ], style={'textAlign': 'center'})
                        ])
                    ]
                )
            )

        # Компактное отображение если вкладок много
        if len(tabs) > 10:
            tab_style = {'height': '40px', 'padding': '5px'}
            selected_style = {'height': '40px', 'padding': '5px', 'backgroundColor': '#3498db', 'color': 'white'}
        else:
            tab_style = None
            selected_style = None

        self.app.layout = html.Div([
            html.H1("📈 Просмотр графиков",
                    style={'textAlign': 'center', 'color': '#2c3e50', 'marginTop': 20}),
            html.Div([
                html.Label("Фильтр файлов: "),
                dcc.Input(
                    id='filter-input',
                    type='text',
                    placeholder='Введите текст для фильтрации...',
                    style={'width': '300px', 'margin': '10px'}
                ),
                html.Button("Обновить список", id='refresh-files', style={'margin': '10px'})
            ], style={'textAlign': 'center'}),
            html.Hr(),
            dcc.Tabs(
                id='tabs',
                value='tab-0',
                children=tabs,
                vertical=False,
                style=tab_style,
                colors={
                    'border': 'lightgray',
                    'primary': '#3498db',
                    'background': '#f8f9fa'
                }
            ),
            html.Div([
                html.P(f"📊 Всего графиков: {len(tabs)}",
                       style={'textAlign': 'center', 'marginTop': 30, 'color': '#7f8c8d'}),
                html.P("💡 Совет: Используйте колесико мыши для масштабирования, двойной клик для сброса",
                       style={'textAlign': 'center', 'fontSize': '12px', 'color': '#95a5a6'})
            ])
        ])

        return self.app

    def run(self, debug=True, port=8050):
        """Запуск приложения"""
        if self.create_app():
            print(f"Запуск сервера на http://localhost:{port}")
            print(f"Найдено графиков: {len(self.files)}")
            self.app.run(debug=debug, port=port)
        else:
            print("Не удалось создать приложение!")


# Использование
if __name__ == '__main__':
    # Простой запуск со всеми .txt файлами
    viewer = PlotlyViewer("*.data")
    viewer.run(debug=True, port=8050)

    # Или с конкретными файлами
    # viewer = PlotlyViewer("data*.txt")  # все файлы начинающиеся с data
    # viewer.run()