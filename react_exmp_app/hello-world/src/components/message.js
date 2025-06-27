import React, { Component } from 'react'

class Message extends Component {

	constructor() {
		super()
		this.state = {
			message: 'Herro anon'
		}
	}

	changeMessage() {
		this.setState({
		message: 'No more anon for you L'
		})
	}

	render() {
	return (
		<div>
		<h1>{this.state.message}</h1>
		<button onClick={() => this.changeMessage()}>Identify Yourself</button>
		</div>
	)
	}
}

export default Message

