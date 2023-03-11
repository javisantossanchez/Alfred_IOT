import requests
from io import BytesIO
from PIL import Image
from datetime import datetime, timedelta
from reportlab.lib.pagesizes import letter
from reportlab.pdfgen import canvas


def get_graph_image(api_key, graph_url, from_time, to_time, width=800, height=400):
    url = graph_url + f'?from={from_time}&to={to_time}&width={width}&height={height}'
    headers = {'Authorization': f'Bearer {api_key}'}
    response = requests.get(url, headers=headers)
    if response.status_code == 200:
        image = Image.open(BytesIO(response.content))
        return image
    else:
        return None


def generate_pdf(graph_urls, api_key, from_time, to_time, file_name):
    c = canvas.Canvas(file_name, pagesize=letter)
    y = 750
    for url in graph_urls:
        image = get_graph_image(api_key, url, from_time, to_time)
        if image:
            c.drawInlineImage(image, 100, y, width=500, height=250)
            y -= 300
    c.save()


api_key = 'TU_CLAVE_DE_API_DE_GRAFANA'
graph_urls = ['https://grafana.com/grafana/dashboards/dashboard1', 'https://grafana.com/grafana/dashboards/dashboard2']
from_time = datetime.utcnow() - timedelta(days=30)
to_time = datetime.utcnow()
file_name = 'informe.pdf'

generate_pdf(graph_urls, api_key, from_time, to_time, file_name)

print(f'Se ha generado el archivo {file_name} correctamente.')
