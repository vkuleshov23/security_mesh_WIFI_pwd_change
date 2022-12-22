var systemMode = document.getElementById("systemMode");
var ignore = document.getElementById("ignoreMode");
var ssid = document.getElementById("ssid");
var password = document.getElementById("password");
let list = document.getElementById("nodeList");


function getWifiChange() {
    fetch("/api/get/WiFiChange")
        .then((d) => {
            return d.json();
        })
        .then(function(d) {
            ssid.innerHTML = d.ssid;
            password.innerHTML = d.password;
            setTimeout(getWifiChange, 500);
        })
        .catch(function(err) {
            console.log(err);
            setTimeout(getWifiChange, 500);
        });
}
getWifiChange();

function getNodeList() {
    fetch("/api/get/nodeList")
        .then((d) => {
            return d.json();
        })
        .then(function(d) {
            while(list.hasChildNodes()){
                list.removeChild('li');
            }
            let nodes = d.nodes;
            nodes.forEach(node => {
                let li = document.createElement("li");
                li.innerText = node;
                list.appendChild(li);
            });
            setTimeout(getNodeList, 1000);
        })
        .catch(function(err) {
            console.log(err);
            setTimeout(getNodeList, 1000);
        });
}
getNodeList();

function getSystemMode() {
    fetch("/api/get/systemMode")
        .then((d) => {
            return d.json();
        })
        .then(function(d) {
            systemMode.value = d.systemMode;
            setTimeout(getSystemMode, 500);
        })
        .catch(function(err) {
            console.log(err);
            setTimeout(getSystemMode, 500);
        });
}
getSystemMode();

function getIgnore() {
    fetch("/api/get/ignoreMode")
        .then((d) => {
            return d.json();
        })
        .then(function(d) {
            ignoreMode.value = d.ignoreMode;
            setTimeout(getIgnore, 500);
        })
        .catch(function(err) {
            console.log(err);
            setTimeout(getIgnore, 500);
        });
}
getIgnore();

function sendSystemMode() {
    console.log("sendSystemMode button was clicked!");
    if (systemMode.innerHTML === "local"){
        systemMode.innerHTML = "center";
    }else{
        systemMode.innerHTML = "local";
    }

    var xhr = new XMLHttpRequest();
    var url = "/api/set/systemMode";
    var param = "systemMode=" + systemMode.innerHTML
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            // Typical action to be performed when the document is ready:
            if (xhr.responseText != null) {
                console.log(xhr.responseText);
            }
        }
    };
    xhr.open("GET", url + "?" + param, true);
    xhr.send();
    
    console.log(JSON.stringify(data));
};

function sendIgnoreMode() {
    console.log("sendIgnore button was clicked!");
    if (ignoreMode.innerHTML === "on"){
        ignoreMode.innerHTML = "off";
    }else{
        ignoreMode.innerHTML = "on";
    }
    var data = {
        ignoreMode: ignoreMode.value,
    };
    var v = 0
    if(ignoreMode.innerHTML == "off"){
        v = 1;
    } 
    var xhr = new XMLHttpRequest();
    var url = "/api/set/ignoreMode";
    var param = "ignoreMode=" + v
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            // Typical action to be performed when the document is ready:
            if (xhr.responseText != null) {
                console.log(xhr.responseText);
            }
        }
    };
    xhr.open("GET", url + "?" + param, true);
    xhr.send();
};


function sendWifiChange() {
    console.log("sendWifiChange button was clicked!");
    
    var data = {
        ignoreMode: ignoreMode.value,
    };
    var xhr = new XMLHttpRequest();
    var url = "/api/set/WiFiChange";
    xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
            // Typical action to be performed when the document is ready:
            if (xhr.responseText != null) {
                console.log(xhr.responseText);
            }
        }
    };
    xhr.open("GET", url, true);
    xhr.send();
};
