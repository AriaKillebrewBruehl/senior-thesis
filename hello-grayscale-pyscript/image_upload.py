from js import document, console, window 
import pyodide
import asyncio 

def _upload_file_and_show(e):
    console.log("Attempted file upload: " + e.target.value)
    file_list = e.target.files
    first_item = file_list.item(0)

    new_image = document.createElement('img')
    new_image.src = window.URL.createObjectUrl(first_item)
    document.getElementById("output_upload").appendChild(new_image)

upload_file = pyodide.create_proxy(_upload_file_and_show)