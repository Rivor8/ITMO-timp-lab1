let correct_password = "96397d44356b443ba1b483026fe78685"; // badpassword122
let current_password = "";

let pass_val = document.getElementById("pass");

let unlock = () => {
    current_password = MD5(pass_val.value);
    if (correct_password == current_password) alert("Контент разблокирован");
    else alert("Контент не разблокирован")
}

document.onselectstart = () => { return correct_password == current_password };
document.ondragstart = () => { return correct_password == current_password };
document.oncontextmenu = () => { return correct_password == current_password };
document.oncopy = () => { return correct_password == current_password };

// window.addEventListener('keypress', (event) => {
//     const keyName = event.key;
  
//     // Приведение к нижнему регистру имени клавиши обязательно
//     // т.к. при нажатии вместе с SHIFT оно будет в верхнем регистре
//     if (event.ctrlKey && event.key.toLowerCase() === 's') {
//         alert('CTRL+SHIFT+T pressed');
//     }
//   });