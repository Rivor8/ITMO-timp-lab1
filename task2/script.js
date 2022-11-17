let correct_password = "96397d44356b443ba1b483026fe78685"; // badpassword122
let current_password = "";

let pass_val = document.getElementById("pass");

let unlock = () => {
    current_password = MD5(pass_val.value);
    if (correct_password == current_password) alert("Контент разблокирован");
    else alert("Контент не разблокирован");
}

document.onselectstart = () => { return correct_password == current_password };
document.ondragstart = () => { return correct_password == current_password };
document.oncontextmenu = () => { return correct_password == current_password };
document.oncopy = () => { return correct_password == current_password };



document.addEventListener('keyup', (e) => {
    if (e.key === 'PrintScreen') {
        if (correct_password != current_password) {
            document.body.hidden = true;
            navigator.clipboard.writeText('');
        }
    }
});
