var ws = undefined;

var onMessageWS = function (ev) {

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
    ws = new WebSocket("ws://localhost:57077/ws");
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