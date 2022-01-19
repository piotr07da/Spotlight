window.addEventListener("DOMContentLoaded", () => {
  const canvas = <HTMLCanvasElement>document.getElementById("canvas");
  const context = canvas.getContext("2d");
  context.fillRect(10, 20, 200, 100);

  console.log(window.samplesApi);
  window.samplesApi.onNext((samples: number[]) =>{
    const canvas = <HTMLCanvasElement>document.getElementById("canvas");
    const ctx = canvas.getContext("2d");
	ctx.clearRect(0, 0, canvas.width, canvas.height);
	ctx.beginPath();
	ctx.moveTo(0, canvas.height / 2);
	for (let i = 0; i < samples.length; ++i) {
      context.lineTo(i * canvas.width / samples.length, canvas.height - canvas.height * samples[i] / 4095);
	}
	ctx.stroke();
  });
});

