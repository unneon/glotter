var ws = undefined;

var min = (a, b) => b < a ? b : a;
var max = (a, b) => b > a ? b : a;

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
            color: "#000",
        });
        s.refresh();
    },
    resize: function (newn) {
        if (newn > graph.n) {
            for (var i=graph.n; i<newn; ++i) {
                s.graph.addNode({
                    id: 'v' + i.toString(),
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
        var node = s.graph.nodes('v'+v.toString());
        node.color = col;
        s.refresh();
    },
    setEdgeColor: function (a, b, col) {
        var edge = graph.findEdge(a, b);
        edge.color = col;
        s.refresh();
    },
    n: 0,
    stateid: 0,
    findEdge: function (a, b) {
        var a1 = 'v' + min(a, b).toString();
        var b1 = 'v' + max(a, b).toString();
        console.log('looking for ' + a1.toString() + ' ' + b1.toString());
        var es = s.graph.edges();
        for (var e of es) {
            var a2 = min(e.source, e.target);
            var b2 = max(e.source, e.target);
            console.log('candidate ' + a2.toString() + ' ' + b2.toString());
            if (a1 == a2 && b1 == b2) {
                console.log('found!');
                return e;
            }
        }
        return undefined;
    }
};

var onMessageWS = function (ev) {
    console.log(ev.data);
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