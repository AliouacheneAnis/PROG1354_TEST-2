// Declation de la constante qui contient la page HTML 

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
    <head>
        <title>Bootstrap Pages</title>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
        <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">
    </head>
    <style>
      .row {
        font-size: 40px;
      }

      .sup {
        font-size: 25px;
      }
    </style>
    <body>

        <div class="container">
            <div class="d-flex justify-content-center p-5">
                <div class="col-lg-auto">
                  <h1>Bienvenue cher Client </h1>
                </div>
              </div>

            <div class="row p-3" style="color: orange;"  >
                <div class="col text-end"> 
                  <i class="fas fa-temperature-high"></i>
                </div>
                <div class="col text-center">
                  Temperature
                </div>
                <div class="col">
                  <span id="temperature"></span><sup>°C</sup>
                </div>
              </div>

              <div class="row p-3"  style="color: blue;">
                <div class="col text-end"> 
                  <i class="fas fa-burn"></i>
                </div>
                <div class="col text-center">
                  Humidity
                </div>
                <div class="col">
                  <span id="humidity"></span><sup>%</sup>
                </div>
              </div>

              <div class="row p-3" style="color:purple;">
                <div class="col text-end"> 
                  <i class="fas fa-gem"></i>
                </div>
                <div class="col text-center">
                  Pressure
                </div>
                <div class="col">
                  <span id="pressure"></span><sup>hPa</sup>
                </div>
              </div>

                <div class="row p-3" style="color: rosybrown; margin-left: 60px;">
                <div class="col text-center mb-3"> 
                  TIME :  <span id="time"></span>
                </div>
              </div>
        </div>
    </body>
    <script>
              
              setInterval(function ( ) {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                  if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("temperature").innerHTML = this.responseText;
                  }
                };
                xhttp.open("GET", "/temperature", true);
                xhttp.send();
              }, 1200 ) ;


              setInterval(function ( ) {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                  if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("humidity").innerHTML = this.responseText;
                  }
                };
                xhttp.open("GET", "/humidity", true);
                xhttp.send();
              }, 1400 ) ;

              setInterval(function ( ) {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                  if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("pressure").innerHTML = this.responseText;
                  }
                };
                xhttp.open("GET", "/pressure", true);
                xhttp.send();
              }, 1600 ) ;

              setInterval(function ( ) {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                  if (this.readyState == 4 && this.status == 200) {
                    document.getElementById("time").innerHTML = this.responseText;
                  }
                };
                xhttp.open("GET", "/time", true);
                xhttp.send();
              },1000 ) ;

</script>
</html>)rawliteral"; 
