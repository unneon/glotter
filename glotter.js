var ws = undefined;

var min = (a, b) => b < a ? b : a;
var max = (a, b) => b > a ? b : a;

var errorHandler = function (msg) {
    // TODO
};

var s = new sigma('graph');

var graph = {
    getEId: (a, b) => 'e'+a.toString()+'_'+b.toString(),
    getVId: v => 'v'+v.toString(),
    addEdge: function (a, b) {
        s.graph.addEdge({
            id: graph.getEId(a, b),
            source: graph.getVId(a),
            target: graph.getVId(b),
            color: "#000",
        });
        s.refresh();
    },
    resize: function (newn) {
        if (newn > graph.n) {
            for (var i=graph.n; i<newn; ++i) {
                s.graph.addNode({
                    id: graph.getVId(i),
                    label: '['+(i+1).toString()+']',
                    x: Math.random(),
                    y: Math.random(),
                    size: 1,
                    color: "#000",
                });
                graph.n = i+1;
            }
        } else if (newn < graph.n) {
            // TODO
        }
        s.refresh();
    },
    setVertexColor: function (v, col) {
        var node = s.graph.nodes(graph.getVId(v));
        node.color = col;
        s.refresh();
    },
    setEdgeColor: function (a, b, col) {
        var edge = s.graph.edges(graph.getEId(a, b));
        edge.color = col;
        s.refresh();
    },
    n: 0,
};

var onMessageWS = function (ev) {
    var cmd = ev.data.split(' ');
    if (cmd[0] === "addEdge") {
        graph.addEdge(parseInt(cmd[1]), parseInt(cmd[2]));
    } else if (cmd[0] === "resize") {
        graph.resize(parseInt(cmd[1]));
    } else if (cmd[0] === "setEdgeColor") {
        graph.setEdgeColor(parseInt(cmd[1]), parseInt(cmd[2]), cmd[3]);
    } else if (cmd[0] === "setVertexColor") {
        graph.setVertexColor(parseInt(cmd[1]), cmd[2]);
    } else {
        errorHandler("Unrecognized message.");
    }
};

var statusBar = function (text, csscls) {
    $('#status').className = csscls;
    $('#status-text').textContent = text;
}

var bind = function (f, ...xs) {
    return function () {
        return f(...xs);
    };
};

// setup WebSocket
(function () {
    if (!("WebSocket" in window)) {
        statusBar('WebSocket not supported.', 'status-unsupported');
        return;
    }
    statusBar('Connecting...', 'status-connecting');
    ws = new WebSocket("ws://localhost:57077");
    ws.onopen = bind(statusBar, 'Ready.', 'status-ready');
    ws.onclose = bind(statusBar, 'Connection closed.', 'status-closed');
    ws.onerror = bind(statusBar, 'WebSocket error.', 'status-error');
    ws.onmessage = onMessageWS;
})();