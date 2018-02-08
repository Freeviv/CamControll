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
    var js_msg = "There where errors in your blabla:<br>"
    if(val == "") {
        document.getElementById("js_msg").innerHTML = "Please select a calculation type!";
    } else {
        if(val == "raw") {

        } else if(val == "finished") {
            var spup  = document.forms["config"]["speedup"].value;
            if(parseInt(spup) <= 0)
            {
                js_msg += "Speedup value too small or negative!<br>"
                document.getElementById("js_msg").innerHTML = js_msg;
            }
        }
    }

    //alert(val);
    return false;
}