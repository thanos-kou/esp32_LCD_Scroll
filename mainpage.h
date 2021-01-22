const char index_html[] PROGMEM = R"rawliteral(
<html lang="en">
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<!-- Add icon library -->
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css">
<title>Upload Tool</title>
<style>
.buttons {
  background-color: #B3001E; 
  border: none;
  color: white;
  padding: 10px 15px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
}
div{background:#33658A;max-width:500px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}
form.a{background:#33658A;max-width:445px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}
form.b{background:#B6B3A5;max-width:350px;margin:75px auto;padding:20px;border-radius:5px;text-align:center}
body{background:White;font-family:sans-serif;font-size:14px;color:white; width: auto; text-align:center}
.btn {
  background-color: black;
  border: none;
  color: white;
  padding: 12px 16px;
  font-size: 16px;
  cursor: pointer;
}

/* Darker background on mouse-over */
.btn:hover {
  background-color: #33658A;
}
</style>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
</head>
<body>
 <button class="btn" onclick="homepage()"><i class="fa fa-home"></i> Home</button>
<div>

<p>Firmware: %FIRMWARE%</p>
  <p>Free Storage: <span id="freespiffs">%FREESPIFFS%</span> | Used Storage: <span id="usedspiffs">%USEDSPIFFS%</span> | Total Storage: <span id="totalspiffs">%TOTALSPIFFS%</span></p>
  <p>
  <button class='buttons' onclick="logoutButton()">Logout</button>
  <button class='buttons' onclick="restart1()">Restart ESP32</button>
  <button class='buttons' onclick='listFilesButton()'>List Files</button>
  <button class='buttons' onclick='showUploadButtonFancy()'>Upload Files</button>
  </p>
  <p id="status"></p>
  <p id="detailsheader"></p>
  <p id="details"></p>
 </div>
<script>
function homepage() {
  location.replace("/")
}
function restart1(){
var xhr = new XMLHttpRequest();
  xhr.open("GET", "/restart1", true);
  xhr.send();
  setTimeout(function(){ window.open("/upload","_self"); }, 10000);
}
function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}
function listFilesButton() {
  xmlhttp=new XMLHttpRequest();
  xmlhttp.open("GET", "/listfiles", false);
  xmlhttp.send();
  document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
  document.getElementById("details").innerHTML = xmlhttp.responseText;
}
function downloadDeleteButton(filename, action) {
  var urltocall = "/file?name=" + filename + "&action=" + action;
  xmlhttp=new XMLHttpRequest();
  if (action == "delete") {
    xmlhttp.open("GET", urltocall, false);
    xmlhttp.send();
    document.getElementById("status").innerHTML = xmlhttp.responseText;
    xmlhttp.open("GET", "/listfiles", false);
    xmlhttp.send();
    document.getElementById("details").innerHTML = xmlhttp.responseText;
  }
  if (action == "download") {
    document.getElementById("status").innerHTML = "";
    window.open(urltocall,"_blank");
  }
}
function showUploadButtonFancy() {
  document.getElementById("detailsheader").innerHTML = "<h3>Upload HTML Files<h3>"
  document.getElementById("status").innerHTML = "";
  var uploadform = "<form method = \"POST\" action = \"/\" enctype=\"multipart/form-data\"><input type=\"file\" name=\"data\"/><input type=\"submit\" name=\"upload\" value=\"Upload\" title = \"Upload File\"></form>"
  document.getElementById("details").innerHTML = uploadform;
  var uploadform =
  "<form class= 'b'><form id=\"upload_form\" enctype=\"multipart/form-data\" method=\"post\">" +
  "<input type=\"file\" name=\"file1\" id=\"file1\" onchange=\"uploadFile()\"><br>" +
  "<progress id=\"progressBar\" value=\"0\" max=\"100\" style=\"width:300px;\"></progress>" +
  "<h3 id=\"status\"></h3>" +
  "<p id=\"loaded_n_total\"></p>" +
  "</form>";
  document.getElementById("details").innerHTML = uploadform;
}
function _(el) {
  return document.getElementById(el);
}
function uploadFile() {
  var file = _("file1").files[0];
  // alert(file.name+" | "+file.size+" | "+file.type);
  var formdata = new FormData();
  formdata.append("file1", file);
  var ajax = new XMLHttpRequest();
  ajax.upload.addEventListener("progress", progressHandler, false);
  ajax.addEventListener("load", completeHandler, false); // doesnt appear to ever get called even upon success
  ajax.addEventListener("error", errorHandler, false);
  ajax.addEventListener("abort", abortHandler, false);
  ajax.open("POST", "/");
  ajax.send(formdata);
}
function progressHandler(event) {
  //_("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes of " + event.total; // event.total doesnt show accurate total file size
  _("loaded_n_total").innerHTML = "Uploaded " + event.loaded + " bytes";
  var percent = (event.loaded / event.total) * 100;
  _("progressBar").value = Math.round(percent);
  _("status").innerHTML = Math.round(percent) + "% uploaded... please wait";
  if (percent >= 100) {
    _("status").innerHTML = "Please wait, writing file to filesystem";
  }
}
function completeHandler(event) {
  _("status").innerHTML = "Upload Complete";
  _("progressBar").value = 0;
  xmlhttp=new XMLHttpRequest();
  xmlhttp.open("GET", "/listfiles", false);
  xmlhttp.send();
  document.getElementById("status").innerHTML = "File Uploaded";
  document.getElementById("detailsheader").innerHTML = "<h3>Files<h3>";
  document.getElementById("details").innerHTML = xmlhttp.responseText;
}
function errorHandler(event) {
  _("status").innerHTML = "Upload Failed";
}
function abortHandler(event) {
  _("status").innerHTML = "inUpload Aborted";
}
</script>
</body>
</html>
)rawliteral";
 const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
</head>
<body>
  <p><a href="/">Go to home page</a></p>
</body>
</html>
)rawliteral";
