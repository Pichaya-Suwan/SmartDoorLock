import firebase from 'firebase';

class Database{
    constructor(){
        var config = {
            databaseURL: "https://embeded-project-866b7.firebaseio.com",
            projectId: "embeded-project-866b7",
        };
        
        if (!firebase.apps.length) {
            firebase.initializeApp(config);
        }
        else{
            console.log("firebase already running ")
        }
    };

    setStatus=(status)=>{
        firebase.database().ref('statusApp')
        .set(status)
        .then((data)=>{
            //success callback
            console.log('data ' , data)
        })
        .catch((error)=>{
            //error callback
            console.log('error ' , error)
        })
    }

    getStatus=(listtennig)=>{
        firebase.database().ref('status')
        .on('value',
        function(snapshot){
            listtennig(snapshot.val())
            //console.log(snapshot.val())      
        });  
    }
}

const database = new Database();
export default database;




