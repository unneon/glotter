var ws = undefined;

var errorHandler = function (msg) {
    // TODO
};

var graphAddEdge = function (a, b) {
    document.getElementById('dumblog').textContent += "addEdge " + a + " " + b + "\n";
};
var graphResize = function (n) {
    document.getElementById('dumblog').textContent += "resize " + n + "\n";
};

var onMessageWS = function (ev) {
    console.log(ev.data);
    var cmd = ev.data.split(' ');
    if (cmd[0] === "addEdge") {
        graphAddEdge(parseInt(cmd[1]), parseInt(cmd[2]));
    } else if (cmd[0] === "resize") {
        graphResize(parseInt(cmd[1]));
    } else {
        errorHandler("Unrecognized message.");
    }
};

// setup WebSocket
(function () {
    var elstat = document.getElementById('status');
    var elstattext = document.getElementById('status-text');
    if (!("WebSocket" in window)) {
        elstat.className = 'status-unsupported';
        elstattext.textContent = 'WebSocket not supported';
        return;
    }
    elstat.className = 'status-connecting';
    elstattext.textContent = 'Connecting...'
    ws = new WebSocket("ws://localhost:57077");
    ws.onopen = function() {
        elstat.className = 'status-ready';
        elstattext.textContent = 'Ready.';
    };
    ws.onclose = function() {
        elstat.className = 'status-closed';
        elstattext.textContent = "Connection closed.";
    };
    ws.onerror = function() {
        elstat.className = 'status-error';
        elstattext.textContent = "WebSocket error.";
    }
    ws.onmessage = onMessageWS;
})();