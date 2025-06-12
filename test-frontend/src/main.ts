import { getNativeFunction } from "juce-framework-frontend-mirror"

function loadVideo(
  element: HTMLAudioElement | HTMLVideoElement,
  context: CanvasRenderingContext2D
) {
  let audioCtx: AudioContext
  // let workletNode: AudioWorkletNode

  const hello = getNativeFunction('hello')
  console.log(hello)
  hello()

  const pushBuffer = getNativeFunction('pushBuffer')
  console.log(pushBuffer)

  const handlePlay = async () => {
    console.log('play video')
    if (audioCtx) {
      console.warn('already existing audio context!')
      return
    }
    audioCtx = new AudioContext()

    const scriptNode = audioCtx.createScriptProcessor()
    console.log(scriptNode.bufferSize)
    scriptNode.onaudioprocess = (event) => {
      const inputBuffer = event.inputBuffer
      const outputBuffer = event.outputBuffer
      const channelCount = inputBuffer.numberOfChannels
      const sampleCount = inputBuffer.length
      const numbersBuffer: number[][] = [[]]

      for (let channel = 0; channel < channelCount; channel++) {
        const inputData = inputBuffer.getChannelData(channel)
        const outputData = outputBuffer.getChannelData(channel)
        const ary: number[] = []
        for (let i = 0; i < inputBuffer.length; i++) {
          // outputData[i] = inputData[i] // bypass
          ary.push(inputData[i])
        }
        numbersBuffer.push(ary)
      }
      pushBuffer(channelCount, sampleCount, numbersBuffer)
    }

    const sourceNode = audioCtx.createMediaElementSource(element)
    const analyser = audioCtx.createAnalyser()
    sourceNode.connect(analyser).connect(scriptNode).connect(audioCtx.destination)
    // sourceNode.connect(workletNode)

    loadCanvas(context, analyser)
  }

  element.addEventListener('play', handlePlay)
}

function loadCanvas(context: CanvasRenderingContext2D, analyser: AnalyserNode) {
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

  let video: HTMLVideoElement | null
  if (IFRAME) {
    const iframe = document.querySelector('iframe')
    video = iframe?.contentWindow?.document.querySelector('video') || null
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
