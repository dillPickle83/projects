import './App.css';
import { Greet } from './components/greet'
import Welcome from './components/welcome'
import Hello from './components/hello'
import Message from './components/message'

function App() {
  return (
    <div className="App">
	<Greet name="Joji" alterEgo="Filthy Frank"/>
			<p>Its filthy Frank</p>
	<Greet name="Elliot" alterEgo="Mr.Robot"/>
			<button>Hecking time</button>
	<Welcome name="Joji" alterEgo="Filthy Frank"/>
	<Hello />
	<Message />
    </div>
  );
}

export default App;
