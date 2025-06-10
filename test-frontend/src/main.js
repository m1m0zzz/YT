/**
 * @param element {HTMLAudioElement | HTMLVideoElement}
 * @param context {CanvasRenderingContext2D}
 */
function loadVideo(
  element,
  context
) {
  /** @type AudioContext */
  let audioCtx
  /** @type AudioWorkletNode */
  // let workletNode

  const handlePlay = async () => {
    console.log('play video')
    if (audioCtx) {
      console.warn('already existing audio context!')
      return
    }
    audioCtx = new AudioContext()

    // console.log('waiting...')
    // await audioCtx.audioWorklet.addModule('http://localhost:5173/src/processors.js')
    // console.log('add module')
    // workletNode = new AudioWorkletNode(
    //   audioCtx,
    //   'my-worklet-processor'
    // )

    const sourceNode = audioCtx.createMediaElementSource(element)
    const analyser = audioCtx.createAnalyser()
    sourceNode.connect(analyser)
    // sourceNode.connect(workletNode)
    sourceNode.connect(audioCtx.destination)

    loadCanvas(context, analyser)
  }

  element.addEventListener('play', handlePlay)
}

/**
 * @param context {CanvasRenderingContext2D}
 * @param analyser {AnalyserNode}
 */
function loadCanvas(context, analyser) {
  console.log('loadCanvas')
  const w = context.canvas.width
  const h = context.canvas.height

  function loop() {
    context.clearRect(0, 0, w, h)
    const bufferLength = analyser.fftSize
    const dataArray = new Float32Array(bufferLength)
    analyser.getFloatTimeDomainData(dataArray)

    const sliceWidth = w / bufferLength
    let x = 0
    context.beginPath()

    for (let i = 0; i < bufferLength; i++) {
      const data = dataArray[i]
      const y = h / 2 + data * h / 2

      if (i === 0) {
        context.moveTo(x, y)
      } else {
        context.lineTo(x, y)
      }
      x += sliceWidth
    }

    context.lineTo(w, h / 2)
    context.stroke()

    requestAnimationFrame(loop)
  }
  loop()
}

// ==========================================================

console.log('hello, user script')

const onload = () => {
  const IFRAME = false
  console.log('onload')

  let video
  if (IFRAME) {
    const iframe = document.querySelector('iframe')
    video = iframe.contentWindow.document.querySelector('video')
  } else {
    video = document.querySelector('video')
  }
  if (!video) {
    console.warn('video tag not find!')
    return
  }

  const canvas = (() => {
    let _canvas = document.querySelector('canvas')
    if (!_canvas) {
      _canvas = document.createElement('canvas')
      _canvas.setAttribute('id', 'my-elm')
      _canvas.width = 300
      _canvas.height = 200
      _canvas.style = 'border: 1px solid rgb(68, 68, 68);z-index: 10000;position: fixed;background: white;'
    }
    return _canvas
  })()

  const ytd = document.querySelector('ytd-app')
  if (ytd) {
    ytd.prepend(canvas)
  } else {
    document.body.appendChild(canvas)
  }
  if (!canvas) {
    console.warn('canvas tag not find!')
    return
  }
  const context = canvas.getContext('2d')
  if (!context) {
    console.warn('can\'t get canvas context!')
    return
  }
  console.log(context)

  loadVideo(video, context)
}

window.addEventListener('load', onload)
