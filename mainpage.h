const char firmware_statusFail[] PROGMEM = R"rawliteral(
<title>Status</title>
<meta  name='viewport' content='width=device-width, initial-scale=1'>
<div0 id="mySidenav" class="sidenav">
<a href="javascript:void(0)" class="closebtn" onclick="closeNav()">&times;</a>
  <h1><a href='/'>Home</a></h1>
<h1><a href='/upload'>Upload Tool</a></h1>
<h1><a href='/BootLoaderIndex'>Firmware Tool</a></h1>
 <h1> <a href='https://www.iee.ihu.gr/'>University Website</a></h1>
 
</div0>
<div1 id="main">
  <body><span style="font-size:30px;cursor:pointer" onclick="openNav()">&#9776; Menu</span></body>
</div1>
<a href='/'><img border='0' src='https://m.902.gr/sites/default/files/styles/902-grid-8/public/MediaV2/20200206/dipae.jpg?itok=kOvwnoTx' width='250' height='150'></a>
<div3 class='a'><form>
<div class=b id=plaisio><body1><h2>WiFi Firmware Update</h2></body1></div><br>
 
  <p>Upload failed.Please try again...</p><br>
  
  </form></div3>



  <script>


function openNav() {
  document.getElementById("mySidenav").style.width = "100%";
}

function closeNav() {
  document.getElementById("mySidenav").style.width = "0";
}
</script>

  <style>
div3.a {
  text-align: center;
}
.b{
background:#33658A;max-width:350px;margin:0px auto;padding:.5px;border-radius:0px;
}
img {display: block;margin-left: auto;margin-right: auto;}
.sidenav {
  height: 100%;
  width: 0;
  position: fixed;
  z-index: 1;
  top: 0;
  left: 0;
  background-color: #111;
  overflow-x: hidden;
  transition: 0.5s;
  padding-top: 60px;
  text-align:center;
}

.sidenav a {
  padding: 8px 8px 8px 32px;
  text-decoration: none;
  font-size: 25px;
  color: #818181;
  display: block;
  transition: 0.3s;

}

.sidenav a:hover{
  color: #f1f1f1;
}

.sidenav .closebtn {
  position: absolute;
  top: 0;
  right: 25px;
  font-size: 36px;
  margin-left: 50px;
}

@media screen and (max-height: 450px) {
  .sidenav {padding-top: 15px;}
  .sidenav a {font-size: 18px;}
}
#main {
  transition: margin-left .5s;
  padding: 16px;
}

body{background:White;font-family:sans-serif;font-size:14px;color:black;}
body1{background:White;font-family:sans-serif;font-size:14px;color:white;width: auto;}


form{background:LightGrey;max-width:350px;margin:75px auto;padding:1px;border-radius:5px;text-align:auto;}

p{
font-size:17px;
}
</style>
)rawliteral";
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

const char firmware_statusOK[] PROGMEM = R"rawliteral(
<title>Status</title>
<meta  name='viewport' content='width=device-width, initial-scale=1'>
<div0 id="mySidenav" class="sidenav">
<a href="javascript:void(0)" class="closebtn" onclick="closeNav()">&times;</a>
  <h1><a href='/'>Home</a></h1>
<h1><a href='/upload'>Upload Tool</a></h1>
<h1><a href='/BootLoaderIndex'>Firmware Tool</a></h1>
 <h1> <a href='https://www.iee.ihu.gr/'>University Website</a></h1>
 
</div0>
<div1 id="main">
  <body><span style="font-size:30px;cursor:pointer" onclick="openNav()">&#9776; Menu</span></body>
</div1>
<a href='/'><img border='0' src='https://m.902.gr/sites/default/files/styles/902-grid-8/public/MediaV2/20200206/dipae.jpg?itok=kOvwnoTx' width='250' height='150'></a>

<div3 class='a'><form>
<div class=b id=plaisio><body1><h2>WiFi Firmware Update</h2></body1></div><br>
 
  <p>Upload Successful...</p><br>
  
  </form></div3>



  <script>


function openNav() {
  document.getElementById("mySidenav").style.width = "100%";
}

function closeNav() {
  document.getElementById("mySidenav").style.width = "0";
}
</script>

  <style>
  img {display: block;margin-left: auto;margin-right: auto;}
div3.a {
  text-align: center;
}
.b{
background:#33658A;max-width:350px;margin:0px auto;padding:.5px;border-radius:0px;
}

.sidenav {
  height: 100%;
  width: 0;
  position: fixed;
  z-index: 1;
  top: 0;
  left: 0;
  background-color: #111;
  overflow-x: hidden;
  transition: 0.5s;
  padding-top: 60px;
  text-align:center;
}

.sidenav a {
  padding: 8px 8px 8px 32px;
  text-decoration: none;
  font-size: 25px;
  color: #818181;
  display: block;
  transition: 0.3s;

}

.sidenav a:hover{
  color: #f1f1f1;
}

.sidenav .closebtn {
  position: absolute;
  top: 0;
  right: 25px;
  font-size: 36px;
  margin-left: 50px;
}

@media screen and (max-height: 450px) {
  .sidenav {padding-top: 15px;}
  .sidenav a {font-size: 18px;}
}
#main {
  transition: margin-left .5s;
  padding: 16px;
}

body{background:White;font-family:sans-serif;font-size:14px;color:black;}
body1{background:White;font-family:sans-serif;font-size:14px;color:white;width: auto;}


form{background:LightGrey;max-width:350px;margin:75px auto;padding:1px;border-radius:5px;text-align:auto;}

p{
font-size:17px;
}
</style>
)rawliteral";
