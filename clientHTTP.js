const url = 'http://192.168.3.222/led';

const bulbImage = document.getElementById('bombillo');
const button = document.getElementById('controlButton');

let ledStatus = false;

button.addEventListener('click', () => {
    ledStatus = !ledStatus;
    const request = ledStatus ? 'on' : 'off';
    updateBulbImage();
    sendRequest(request);
});

const updateBulbImage = () => {
    if (ledStatus) {
        button.textContent = 'Apagar';
    } else {
        button.textContent = 'Encender';
    }
    bulbImage.style.backgroundImage = ledStatus ? "url('bombillo-encendido.png')" : "url('bombillo-apagado.png')";
};

const sendRequest = (request) => {
    fetch(url, {
        method: 'PUT',
        body: request,
        headers: {
            'Content-Type': 'text/plain'
        },
    })
    .then(response => {
        if (response.ok) {
            console.log('Request sent successfully');
        } else {
            console.error('Failed to send request');
        }
    })
    .catch(error => {
        console.error('Error sending request:', error);
    })
}

const obtenerEstadoBombillo = () => {
    
    fetch(url, {
        method: 'GET',
    })
    .then(response => response.text())
    .then(data => {
        ledStatus = data === 'on' ? true : false;
        updateBulbImage();
    })
    .catch(error => {
        console.error('Error fetching bulb status:', error);
    })
}

window.onload = obtenerEstadoBombillo;