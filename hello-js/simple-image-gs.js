var image = null;

// upload the file to the canvas
function upload(){
  var fileinput = document.getElementById("finput");
  image = new SimpleImage(fileinput);
  var canvas = document.getElementById("can");
  image.drawTo(canvas);
}

function makeGray() {
  // aberage pixels
  for (var pixel of image.values()) {
    var avg = (pixel.getRed()+pixel.getGreen()+pixel.getBlue())/3;
    pixel.setRed(avg);
    pixel.setGreen(avg);
    pixel.setBlue(avg);
  }
  // display new image
  var canvas = document.getElementById("can");
  image.drawTo(canvas);
}