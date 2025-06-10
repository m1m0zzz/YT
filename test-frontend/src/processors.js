class MyWorkletProcessor extends AudioWorkletProcessor {
  constructor() {
    super()
  }

  /**
   * @param inputs {Float32Array[][]}
   * @param outputs {Float32Array[][]}
   * @param parameters {Record<string, Float32Array>}
   * @returns
   */
  process(inputs, outputs, parameters) {
    const input = inputs[0]
    const output = outputs[0]

    for (let channel = 0; channel < input.length; channel++) {
      let inputChannel = input[channel]
      let outputChannel = output[channel]
      for (let i = 0; i < inputChannel.length; ++i) {
        outputChannel[i] = inputChannel[i]
        // TODO: add ring buffer
      }
    }

    return true
  }
}

registerProcessor('my-worklet-processor', MyWorkletProcessor)
