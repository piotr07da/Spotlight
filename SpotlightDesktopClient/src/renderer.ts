window.addEventListener("DOMContentLoaded", () => {
	const canvas = <HTMLCanvasElement>document.getElementById("samples-canvas");
	const context = canvas.getContext("2d");
	context.fillRect(10, 20, 200, 100);

	window.spotlightApi.onNextSamples((samples: number[]) => {
		const canvas = <HTMLCanvasElement>document.getElementById("samples-canvas");
		const ctx = canvas.getContext("2d");
		ctx.clearRect(0, 0, canvas.width, canvas.height);
		ctx.strokeStyle = "#0000FF";
		ctx.lineWidth = 2;
		ctx.strokeRect(0, 0, canvas.width, canvas.height);
		ctx.strokeStyle = "#AAAAAA";
		ctx.lineWidth = 1;
		for (let v = 0; v <= 3.3; v += 0.1) {
			ctx.beginPath();
			const y = (canvas.height * v) / 3.3;
			ctx.moveTo(0, y);
			ctx.lineTo(canvas.width, y);
			ctx.stroke();
		}
		ctx.strokeStyle = "#000000";
		ctx.beginPath();
		ctx.moveTo(0, canvas.height / 2);
		for (let i = 0; i < samples.length; ++i) {
			ctx.lineTo((i * canvas.width) / samples.length, canvas.height - (canvas.height * samples[i]) / 4095);
		}
		ctx.stroke();
	});

	window.spotlightApi.onNextSpectrum((spectrum: number[]) => {
		const max = Math.max(...spectrum);
		spectrum = spectrum.map((s) => s / max);

		const canvas = <HTMLCanvasElement>document.getElementById("spectrum-canvas");
		const ctx = canvas.getContext("2d");
		ctx.clearRect(0, 0, canvas.width, canvas.height);
		ctx.strokeStyle = "#0000FF";
		ctx.lineWidth = 2;
		ctx.strokeRect(0, 0, canvas.width, canvas.height);
		ctx.strokeStyle = "#000000";
		ctx.lineWidth = 1;
		ctx.beginPath();
		ctx.moveTo(0, 0);
		for (let i = 0; i < spectrum.length; ++i) {
			ctx.lineTo((i * canvas.width) / spectrum.length, canvas.height - canvas.height * spectrum[i]);
		}
		ctx.stroke();
	});

	window.spotlightApi.onNextSamples((samples: number[]) => {
		const dataSizeElement = document.getElementById("samples-data-size");
		dataSizeElement.innerText = samples.length.toString();
		const dataElement = document.getElementById("samples-data");
		dataElement.innerText = samples.join("\n");
	});

	const peaks: { v: number; ix: number }[] = [];

	window.spotlightApi.onNextSpectrum((values: number[]) => {
		const dataSizeElement = document.getElementById("spectrum-data-size");
		dataSizeElement.innerText = values.length.toString();
		const dataElement = document.getElementById("spectrum-data");
		dataElement.innerText = values.join("\n");

		let maxValue = 0;
		let maxValueIx = 0;
		for (let i = 0; i < values.length; ++i) {
			const v = values[i];
			if (v > maxValue) {
				maxValue = v;
				maxValueIx = i;
			}
		}

		if (maxValue > 30000) {
			peaks.push({ v: maxValue, ix: maxValueIx });
			const peaksElement = document.getElementById("spectrum-peaks");
			peaksElement.innerText = peaks.map((p) => "ix:" + p.ix + " v:" + p.v).join("\n");
		}
	});
});
