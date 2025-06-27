import React from 'react'

// The below code is the way we would render using JSX (JavaScript XML)
//const Hello = () => {
//	return (
//		<div className="dummyclass">
//			<h1>Herro Niggasaki</h1>
//		<div>
//	)
//}

const Hello = () => {
	return React.createElement(
		"div",
		// The below arg can be null if we don't want anything associated with the div component that we're creating. But whatever we do specify will become an attribute of the div class. '<div id="Hello">'
		{id: "Hello", className: "TempClass"},
		React.createElement("h1", null, "Herro Niggasaki")
	)
}

export default Hello

