import { getNativeFunction } from "juce-framework-frontend-mirror"

import workletURL from './processors.ts?url'

function loadVideo(
  element: HTMLAudioElement | HTMLVideoElement,
  context: CanvasRenderingContext2D
) {
  let audioCtx: AudioContext
  let workletNode: AudioWorkletNode

  const handler = async () => {
    if (audioCtx) return
    audioCtx = new AudioContext()

    await audioCtx.audioWorklet.addModule(workletURL)
    console.log('add module')
    workletNode = new AudioWorkletNode(
      audioCtx,
      'my-worklet-processor'
    )

    const sourceNode = audioCtx.createMediaElementSource(element)
    const analyser = audioCtx.createAnalyser()
    sourceNode.connect(analyser)
    sourceNode.connect(workletNode)
    sourceNode.connect(audioCtx.destination)

    loadCanvas(context, analyser)
  }

  element.addEventListener('play', handler)
}

function loadCanvas(context: CanvasRenderingContext2D, analyser: AnalyserNode) {
  const w = context.canvas.width
  const h = context.canvas.height

  function loop() {
    context.clearRect(0, 0, w, h)
    const bufferLength = analyser.fftSize
    const dataArray: Float32Array<ArrayBuffer> = new Float32Array(bufferLength)
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
  console.log('onload')
  const video = document.querySelector('video')
  if (!video) {
    console.warn('video tag not find!')
    return
  }
  // video.muted = true
  console.log(video)

  const canvas = document.querySelector('canvas')
  console.log(canvas)
  if (!canvas) return
  const context = canvas.getContext('2d')
  if (!context) return

  loadVideo(video, context)
}

window.addEventListener('load', onload)
