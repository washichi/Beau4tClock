const form = document.querySelector('form')
const sliders = document.querySelectorAll('input[type="range"]')
const toggleButton = document.getElementById('theme-toggle');

toggleButton.addEventListener('click', function() {
  if(document.documentElement.getAttribute('data-theme') == 'light'){
    document.documentElement.setAttribute('data-theme', 'dark');
  }
    
else {        
  document.documentElement.setAttribute('data-theme', 'light');    
}   
});


const rangeToPercent = slider => {
  const max = slider.getAttribute('max') || 10
  const percent = slider.value / max * 100

  return `${parseInt(percent)}%`
}

sliders.forEach(slider => {
  slider.style.setProperty('--track-fill', rangeToPercent(slider))

  slider.addEventListener('input', e => {
    e.target.style.setProperty('--track-fill', rangeToPercent(e.target))
  })
})

form.addEventListener('input', e => {
  const formData = Object.fromEntries(new FormData(form))
  console.table(formData)
})

function updateFCBrightness(element) {
  var sliderValue = document.getElementById("fc-brightness").value;
  document.getElementById("forecast-brightness-value").innerHTML = sliderValue;
  console.log(sliderValue);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/fc-brightness?value=" + sliderValue, true);
  xhr.send();
}