import { StatusBar } from 'expo-status-bar';
import React, { Component } from 'react';
import { StyleSheet, Text, View , TouchableOpacity} from 'react-native';
import Constants from 'expo-constants';

import { MaterialCommunityIcons } from '@expo/vector-icons';
import database from './firebase/Database'

//import BackgroundJob from 'react-native-background-actions';

class App extends Component{
  constructor(props){
    super(props);
    this.state = {
      status:false,
      textstat:"LOCKED",
      textbut:"UNLOCKED",
      namelock:"lock-outline",
      colorlock:"white"
    }
  }

  onPress =()=>{
    if(this.state.status == false){

      this.setState({status:true})

      //this.setState({colorlock:"green"})
      database.setStatus(this.state.status);   
    }
    else{

      this.setState({status:false})

      //this.setState({colorlock:"red"})
      database.setStatus(this.state.status)
    }
    //database.getStatus();

    
  }

  listenningData=(val)=>{
    console.log(val);
    if(val){
      this.setState({textstat:"LOCKED"})
      //this.setState({textbut:"UNLOCKED"})
      this.setState({namelock:"lock-outline"})
      //database.setStatus(true);
      
    }
    else{
      this.setState({textstat:"UNLOCKED"})
      //this.setState({textbut:"LOCKED"})
      this.setState({namelock:"lock-open-variant-outline"})
      //database.setStatus(false);
    }
    
    
  }
  componentWillMount(){
    database.getStatus(this.listenningData);
  }

  render(props){
    return(
      <View style = {styles.container}>
        <View style = {styles.top}>
          <TouchableOpacity style = {styles.circleLock} onPress = {this.onPress} >
            <MaterialCommunityIcons name={this.state.namelock} size={70} color = {this.state.colorlock} />
          </TouchableOpacity>
          {/* <Text style = {{color:"#E0E0E0"}}>Tap to the lock to {(this.state.textbut).toLocaleLowerCase()}</Text> */}
          <Text style = {{fontSize:30 , color:"black"}}>{this.state.textstat}</Text>
        </View>

        {/* <View style = {styles.down}>
          <Text style = {{color:"#E0E0E0"}}>Tap to {(this.state.textbut).toLocaleLowerCase()}</Text>
          <TouchableOpacity style = {styles.button} onPress = {this.onPress}>
            <Text style = {{fontSize:35,color:"white"}}>{this.state.textbut}</Text>
          </TouchableOpacity>
        </View> */}
      </View>
    );
  }
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    //backgroundColor: '#fff',
    //justifyContent:"center",
    //alignItems:"center",
    paddingTop:Constants.statusBarHeight,

  },
  button:{
    backgroundColor:'#BACEFD',
    //padding:10,
    //paddingHorizontal:20,
    //paddingVertical:10,
    borderRadius:10,
    height:50,
    width:150,
    justifyContent:"center",
    alignItems:"center",
    marginTop:10
  },
  top:{
    flex:5,
    justifyContent:"center",
    alignItems:"center",
  },
  down:{
    flex:1,
    alignItems:"center",
  },
  circleLock:{
    height:300,
    width:300,
    borderRadius:200,
    backgroundColor:"#BAEDFD",
    justifyContent:"center",
    alignItems:"center",
    marginBottom:30
  }
});

export default App;