const img = document.getElementById("finput");
const canvas = document.getElementById("can");
const ctx = canvas.getContext("2D");

function vanillaMakeGray() {
    const imgData = ctx.getImageData(0, 0, canvas.width, canvas.height);
    for (i = 0; i < imgData.data.length; i += 4) {
        let red = imgData.data[i];
        let green = imgData.data[i + 1];
        let blue = imgData.data[i + 2];

        let avg = (red + green + blue) / 3;

        imgData.data[i]     = avg;
        imgData.data[i + 1] = avg;
        imgData.data[i + 2] = avg;
    }

    ctx.putImageData(imgData, 0, 0);
}