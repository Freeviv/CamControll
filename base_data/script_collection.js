
function openCity(evt, cityName) {
    // Declare all variables
    var i, tabcontent, tablinks;

    // Get all elements with class="tabcontent" and hide them
    tabcontent = document.getElementsByClassName("tabcontent");
    for (i = 0; i < tabcontent.length; i++) {
        tabcontent[i].style.display = "none";
    }

    // Get all elements with class="tablinks" and remove the class "active"
    tablinks = document.getElementsByClassName("tablinks");
    for (i = 0; i < tablinks.length; i++) {
        tablinks[i].className = tablinks[i].className.replace(" active", "");
    }

    // Show the current tab, and add an "active" class to the button that opened the tab
    document.getElementById(cityName).style.display = "block";
    evt.currentTarget.className += " active";
}

function directControl(operator, value)
{
    dc_form = document.createElement('form');
    dc_form.method = "post";
    newInput1 = document.createElement('input');
    newInput1.type = 'hidden';
    newInput1.name = 'operator';
    newInput1.value = operator;
    newInput2 = document.createElement('input');
    newInput2.type = 'hidden';
    newInput2.name = 'value';
    newInput2.value = value;
    
    dc_form.appendChild(newInput1);
    dc_form.appendChild(newInput2);

    document.getElementById('hidden_form_container').appendChild(dc_form);
    dc_form.submit();
}