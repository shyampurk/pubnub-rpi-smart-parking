// Codes represented as array i.e. codeColor[0] corresponds to "available"
var codeColor = ["available", "occupied", "unavailable"]


var app = {
    initialize: function() {
        this.bindEvents();
        this.pubNubInit();
    },
    bindEvents: function() {
        document.addEventListener('deviceready', this.onDeviceReady, false);
    },
    onDeviceReady: function() {
        app.receivedEvent('deviceready');
    },
    // Update DOM on a Received Event
    receivedEvent: function(id) {
        console.log('Received Event: ' + id);
    },

    pubNubInit: function() {

        pubnub = PUBNUB({
            publish_key: 'demo',
            subscribe_key: 'demo'
        })

        console.log("Subscribing..");
        pubnub.subscribe({
            channel: "parkingstatus-resp",
            message: function(message, env, ch, timer, magic_ch) {

                var currentStatus = Object.keys(message).reverse().map(function(key) {
                    return codeColor[message[key]]
                })
                $('.parking-spot').each(function(i, elem) {
                    $(elem).removeClass("available unavailable occupied").addClass(currentStatus[i])
                })

            },
            connect: pub
        })

        function pub() {
            console.log("Since we’re publishing on subscribe connectEvent, we’re sure we’ll receive the following publish.");
            pubnub.publish({
                channel: "parkingstatus-req",
                message: {
                    "Requester": "APP",
                    "Device ID": 0,
                    "Request Type": 4,
                    "Request Value": 0
                },
                callback: function(m) {
                    console.log(m)
                }
            })
        }
    }
};

app.initialize();