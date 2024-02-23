

function login(email, pwd, redirect){
    const xhr = new XMLHttpRequest();
    xhr.open("POST", "/login", true);
    xhr.setRequestHeader('Content-Type', 'application/json');
    xhr.addEventListener("load", (ev)=>{
        window.location.href = redirect;
        console.log(redirect);
    });
    xhr.send(JSON.stringify({
        email: email,
        password: pwd,
        redirect: redirect
    }));
}

function setCookie(name, value, ms) {
    let expires;
    if (ms) {
        const date = new Date();
        date.setTime(date.getTime() + ms);
        expires = "; expires=" + date.toUTCString();
    }
    else {
        expires = "";
    }
    document.cookie = name + "=" + value + expires + "; path=/";
}

function getCookie(cname) {
    const name = cname + "=";
    const decodedCookie = decodeURIComponent(document.cookie);
    const ca = decodedCookie.split(';');
    for(let i = 0; i <ca.length; i++) {
        let c = ca[i];
        while (c.charAt(0) === ' ') {
            c = c.substring(1);
        }
        if (c.indexOf(name) === 0) {
            return c.substring(name.length, c.length);
        }
    }
    return "";
}

function deleteCookie(name) {
    document.cookie = name +'=; Path=/; Expires=Thu, 01 Jan 1970 00:00:01 GMT;';
}

function getLocation(refObject) {
    if (navigator.geolocation) {
        navigator.geolocation.getCurrentPosition(function (position) {
            refObject.invokeMethodAsync("GetCoords", position.coords.latitude + ", " + position.coords.longitude);
        });
    } else {
        refObject.invokeMethodAsync("GetCoords", "");
    }
}

function returnRes(){
    
}
