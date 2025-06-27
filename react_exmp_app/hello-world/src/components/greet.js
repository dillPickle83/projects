import React from 'react'

// Writing a functional component

//function Greet() {
//	return <h1>Konnichiwa My Nigga</h1>
//}

// export default Greet

export const Greet = (props) => {
	console.log(props)
	return(
		<div>
			<h1>Konnichiwa My Nigga {props.name} or shall I call you {props.alterEgo}</h1>
			{props.children}
		</div>
	)
}
// Exporting it like the above statments means that in the app.js file, when we import it we can't really use a different name and the function needs to be imported as 'Greet'. If we had declared the constant without the export in the same line and with 'export default Greet' in the next line, we can import the function with any name and doesn't have to be Greet.
// We can pass in props in the function and that will take the value that we're passing in app.js and will use it here.
// The props.children only renders if the call in app.js has a subtag within the Greet tag. <Greet>...<p></p?</Greet>

