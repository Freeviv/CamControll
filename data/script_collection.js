
function onLoad() {
    document.getElementById("start").disabled = true;
}

function enableSpeedup(enable) {
    if(enable) {
        document.getElementById("speedUp").style.visibility = 'hidden';
        document.getElementById("secs").style.visibility = 'visible';
        document.getElementById("rate").innerHTML = "Take image every";
    } else {
        document.getElementById("speedUp").style.visibility = 'visible';
        document.getElementById("secs").style.visibility = 'hidden';
        document.getElementById("rate").innerHTML = "Framerate";
    }
}
function validateForm() {
    document.getElementById("js_msg").innerHTML = "";
    var fps = document.forms["config"]["fps"].value;
    var length  = document.forms["config"]["length"].value;

    var val = document.forms["config"]["ttype"].value;
    var js_msg = "There where errors in your specifications:<br><ul type=\"square\">"
    var errored = false;
    var spup;

    if(fps == "" || parseInt(fps) <= 0) {
        js_msg += "<li>Frames per second empty or too small</li>"
        var errored = true;
    }

    if(length == "" || parseInt(length) <= 0) {
        js_msg += "<li>Length empty or too small</li>";
        var errored = true;
    }

    if(val == "") {
        document.getElementById("js_msg").innerHTML = "Please select a calculation type!";
    } else {
        if(val == "raw") {

        } else if(val == "finished") {
            spup = document.forms["config"]["speedup"].value;
            if(parseInt(spup) <= 0)
            {
                js_msg += "<li>Speedup value too small or negative</li>"
                var errored = true;
            }
        }
    }
    if(!errored)
    {
        js_msg = "This configuration is valid!<br><ul>";
        var time;
        var img_s
        if(val == "raw")
        {
            time = parseFloat(fps) * parseInt(length);
            img_s = parseInt(fps);
        } else {
            time = parseFloat(length) * parseInt(spup);
            img_s = parseInt(length) / parseFloat(fps);
        }
        js_msg += "<li>This configuration will run for " + time + " seconds</li>";
        js_msg += "or " + time/60 + " minutes<br>";
        js_msg += "or " + time/60/60 + " hours<br>";
        js_msg += "or " + time/60/60/24 + " days";
        js_msg += "<li>and will take every " + img_s + " seconds an image.</li><br>";
        document.getElementById("start").disabled = false;
        errored = true;
    }
    if(errored)
    {
        js_msg += "</ul>"
        document.getElementById("js_msg").innerHTML = js_msg;
    }

    //alert(val);
    return false;
}