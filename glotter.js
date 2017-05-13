var ws = undefined;

var errorHandler = function (msg) {
    // TODO
};

var s = new sigma('graph');

var graph = {
    addEdge: function (a, b) {
        var id = graph.stateid;
        graph.stateid = id + 1;
        s.graph.addEdge({
            id: 'e'+id.toString(),
            source: 'v'+a.toString(),
            target: 'v'+b.toString(),
        });
        s.refresh();
    },
    resize: function (newn) {
        if (newn > graph.n) {
            for (var i=graph.n; i<newn; ++i) {
                s.graph.addNode({
                    id: 'v' + i.toString(),
                    label: (i+1).toString(),
                    x: Math.random(),
                    y: Math.random(),
                    size: 1,
                });
                graph.n = i+1;
            }
        } else if (newn < graph.n) {
            // TODO
        }
        s.refresh();
    },
    n: 0,
    stateid: 0,
};

var onMessageWS = function (ev) {
    console.log(ev.data);
    var cmd = ev.data.split(' ');
    if (cmd[0] === "addEdge") {
        graph.addEdge(parseInt(cmd[1]), parseInt(cmd[2]));
    } else if (cmd[0] === "resize") {
        graph.resize(parseInt(cmd[1]));
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