var ws = undefined;

var min = (a, b) => b < a ? b : a;
var max = (a, b) => b > a ? b : a;

var errorHandler = function (msg) {
    // TODO
};

// TODO: var s

var graph = {
    getEId: (a, b) => 'e'+a.toString()+'_'+b.toString(),
    getVId: v => 'v'+v.toString(),
    addEdge: function (a, b) {

    },
    resize: function (newn) {
        if (newn > graph.n) {
            for (var i=graph.n; i<newn; ++i) {
                // TODO: add vertex
                graph.n = i+1;
            }
        } else if (newn < graph.n) {
            // TODO: downsize graph
		}
        // TODO: refresh
    },
    setVertexColor: function (v, col) {
        // TODO: set vertex color
    },
    setEdgeColor: function (a, b, col) {
        // TODO: set edge color
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