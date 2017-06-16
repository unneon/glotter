var ws = undefined;

var min = (a, b) => b < a ? b : a;
var max = (a, b) => b > a ? b : a;

var errorHandler = function (msg) {
	console.log(msg);
	alert(msg);
}

var graph = {
	nodes: new vis.DataSet([
	]),
	edges: new vis.DataSet([
	]),
	container: document.getElementById('graph'),
	options: {},
    addEdge1: function (a, b) {
		graph.edges.add({
			from: a,
			to: b,
			id: a+'->'+b,
			arrows: {
				to: {
					enabled: true
				}
			},
			color: {
				color: "#000000",
				highlight: "#000000"
			}
		});
    },
	addEdge2: function (a, b) {
		if (a > b) { t = a; a = b; b = t; }
		graph.edges.add({
			from: a,
			to: b,
			id: a+'<->'+b,
			color: {
				color: "#000000",
				highlight: "#000000"
			}
		});
    },
	createVertex: function() {
		graph.nodes.add({
			id: graph.n,
			label: (graph.n+1).toString(),
			color: {
				border: '#000000',
				background: '#ffffff',
				highlight: {
					border: '#000000',
					background: '#ffffff'
				}
			}
		});
		graph.n += 1;
	},
    resize: function (newn) {
        if (newn > graph.n) {
            for (var i=graph.n; i<newn; ++i) {
				graph.createVertex();
            }
        } else if (newn < graph.n) {
            // TODO: downsize graph
		}
    },
    setVertexColor: function (v, col) {
		var node = graph.nodes.get(v);
		node.color.background = col;
		node.color.highlight.background = col;
		graph.nodes.update(node);
    },
    setEdge1Color: function (a, b, col) {
		var edge = graph.edges.get(a+'->'+b);
		edge.color.color = col;
		edge.color.highlight = col;
		graph.edges.update(edge);
    },
	setEdge2Color: function (a, b, col) {
		if (a > b) { t = a; a = b; b = t; }
		var edge = graph.edges.get(a+'<->'+b);
		edge.color.color = col;
		edge.color.highlight = col;
		graph.edges.update(edge);
    },
    n: 0,
};
graph.data = {nodes: graph.nodes, edges: graph.edges};
graph.network = new vis.Network(graph.container, graph.data, graph.options);

var onMessageWS = function (ev) {
    var cmd = ev.data.split(' ');
	// TODO: change this to a map or object[propertyname] call or something
	if (cmd[0] === "pause") {
		remote.pause();
	} else if (cmd[0] === "addEdge1") {
        graph.addEdge1(parseInt(cmd[1]), parseInt(cmd[2]));
	} else if (cmd[0] === "addEdge2") {
		graph.addEdge2(parseInt(cmd[1]), parseInt(cmd[2]));
    } else if (cmd[0] === "resize") {
        graph.resize(parseInt(cmd[1]));
    } else if (cmd[0] === "setEdge1Color") {
        graph.setEdge1Color(parseInt(cmd[1]), parseInt(cmd[2]), cmd[3]);
	} else if (cmd[0] === "setEdge2Color") {
		graph.setEdge2Color(parseInt(cmd[1]), parseInt(cmd[2]), cmd[3]);
    } else if (cmd[0] === "setVertexColor") {
        graph.setVertexColor(parseInt(cmd[1]), cmd[2]);
    } else {
        errorHandler("Unrecognized message.");
    }
};

var statusBar = function (text, csscls) {
    $('#status').attr('class', csscls);
    $('#status-text').text(text);
}
var nextButton = function (text, csscls) {
	$('#next').attr('class', csscls);
	$('#next-text').text(text);
}

var remote = {
	unpause: function() {
		if (remote.isPaused) {
			remote.isPaused = false;
			nextButton('', 'next-off');
			ws.send("unpause");
		}
	},
	pause: function() {
		remote.isPaused = true;
		nextButton('next', 'next-on');
	},
	isPaused: false
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
    ws.onopen = bind(statusBar, 'Ready', 'status-ready');
    ws.onclose = bind(statusBar, 'Closed', 'status-closed');
    ws.onerror = bind(statusBar, 'Error', 'status-error');
    ws.onmessage = onMessageWS;
})();
