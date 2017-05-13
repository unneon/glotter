var ws = undefined;

var errorHandler = function (msg) {
    // TODO
};

var σ = new sigma('graph');

    σ.graph.addNode({
      // Main attributes:
      id: 'n0',
      label: 'Hello',
      // Display attributes:
      x: 0,
      y: 0,
      size: 1,
      color: '#f00'
    }).addNode({
      // Main attributes:
      id: 'n1',
      label: 'World !',
      // Display attributes:
      x: 1,
      y: 1,
      size: 1,
      color: '#00f'
    }).addEdge({
      id: 'e0',
      // Reference extremities:
      source: 'n0',
      target: 'n1'
    });
    σ.refresh();

var graph = {
    addEdge: function (a, b) {
        var id = graph.stateid;
        graph.stateid = id + 1;
        σ.graph.addEdge({
            id: 'e'+id.toString(),
            source: 'v'+a.toString(),
            target: 'v'+b.toString(),
        });
        σ.refresh();
    },
    resize: function (newn) {
        if (newn > graph.n) {
            for (var i=graph.n; i<newn; ++i) {
                σ.graph.addNode({
                    id: 'v' + i.toString(),
                    label: (i+1).toString(),
                    x: 0,
                    y: 0,
                    size: 1,
                    color: '#f00',
                });
                graph.n = i+1;
            }
        } else if (newn < graph.n) {
            // TODO
        }
        σ.refresh();
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