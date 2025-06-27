import React, { Component } from 'react'

// Writing a class component
class Welcome extends Component {
	render() {
		return <h1>Welcum {this.props.name} or shall I call you {this.props.alterEgo}</h1>
	}
}

export default Welcome

