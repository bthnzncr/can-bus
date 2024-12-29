from fastapi import FastAPI, HTTPException
from fastapi.responses import HTMLResponse, JSONResponse
from fastapi.middleware.cors import CORSMiddleware
from fastapi.staticfiles import StaticFiles
import uvicorn
from typing import Dict, List, Optional
from pydantic import BaseModel
import json
from datetime import datetime, timedelta
import requests

app = FastAPI(title="Vehicle Monitoring API")

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

latest_vehicle_info = None
latest_realtime_data = None

last_vehicle_info_update = None
last_realtime_data_update = None

DATA_TIMEOUT = 10

class PIDData(BaseModel):
    name: str
    value: str
    category: Optional[str] = None

class VehicleData(BaseModel):
    message_type: str
    pids: List[PIDData]

def decode_vin(vin):
    """Decode VIN using NHTSA API"""
    try:
        
        vin_url = f'https://vpic.nhtsa.dot.gov/api/vehicles/DecodeVinValues/{vin}?format=json'
        vin_response = requests.get(vin_url)
        vin_data = vin_response.json()
        
        if not vin_data.get('Results'):
            raise Exception("Failed to decode VIN")

        
        result = vin_data['Results'][0]

      
        pids = []
        
        
        pids.append({
            "name": "VIN",
            "value": vin,
            "category": "vehicle_info"
        })

       
        if result.get('VehicleType'):
            pids.append({
                "name": "VEHICLE_TYPE",
                "value": result['VehicleType'],
                "category": "vehicle_info"
            })

        if result.get('Series'):
            series_value = result['Series']
            if result.get('Series2'):
                series_value = f"{series_value} - {result['Series2']}"
            pids.append({
                "name": "SERIES",
                "value": series_value,
                "category": "vehicle_info"
            })

        if result.get('Manufacturer'):
            pids.append({
                "name": "MANUFACTURER",
                "value": result['Manufacturer'],
                "category": "vehicle_info"
            })

        if result.get('PlantCompanyName'):
            pids.append({
                "name": "PLANT_COMPANY",
                "value": result['PlantCompanyName'],
                "category": "vehicle_info"
            })

        # Add plant location information
        if result.get('PlantCity'):
            pids.append({
                "name": "PLANT_CITY",
                "value": result['PlantCity'],
                "category": "vehicle_info"
            })

        if result.get('PlantState'):
            pids.append({
                "name": "PLANT_STATE",
                "value": result['PlantState'],
                "category": "vehicle_info"
            })

        if result.get('PlantCountry'):
            pids.append({
                "name": "PLANT_COUNTRY",
                "value": result['PlantCountry'],
                "category": "vehicle_info"
            })

        if result.get('Model'):
            pids.append({
                "name": "MODEL",
                "value": result['Model'],
                "category": "vehicle_info"
            })

        if result.get('GVWR'):
            pids.append({
                "name": "GVWR",
                "value": result['GVWR'].split("(")[0].strip(),
                "category": "vehicle_info"
            })

        if result.get('DriveType'):
            pids.append({
                "name": "DRIVE_TYPE",
                "value": result['DriveType'],
                "category": "vehicle_info"
            })

        if result.get('Doors'):
            pids.append({
                "name": "DOORS",
                "value": result['Doors'],
                "category": "vehicle_info"
            })

        if result.get('BodyClass'):
            pids.append({
                "name": "BODY_CLASS",
                "value": result['BodyClass'],
                "category": "vehicle_info"
            })

        

        return pids
    except Exception as e:
        print(f"Error decoding VIN: {e}")
        return []

@app.get("/", response_class=HTMLResponse)
async def get_index():
    with open('index.html', 'r') as f:
        return HTMLResponse(content=f.read())

@app.post("/api/vehicle-data")
async def update_vehicle_data(data: VehicleData):
    
    print(f"Received vehicle data: {data.json()}")

    global latest_vehicle_info, latest_realtime_data, last_vehicle_info_update, last_realtime_data_update
    
    if data.message_type == "vehicle_info":
        
        if latest_vehicle_info is None:
            latest_vehicle_info = {"message_type": "vehicle_info", "pids": []}
            print("Initialized latest_vehicle_info")

        existing_pids = {pid["name"]: pid for pid in latest_vehicle_info.get("pids", [])}
        
        
        for pid in data.pids:
            pid_dict = pid.dict()
            print(f"Processing PID: {pid_dict}")
            
        
            if pid.name == "VIN":
                decoded_pids = decode_vin(pid.value)
                if decoded_pids:
                    print(f"Decoded VIN data: {decoded_pids}")
                    # Update existing PIDs with decoded VIN info
                    for decoded_pid in decoded_pids:
                        existing_pids[decoded_pid["name"]] = decoded_pid
            
            
            existing_pids[pid.name] = pid_dict
        
    
        latest_vehicle_info = {
            "message_type": "vehicle_info",
            "pids": list(existing_pids.values())
        }
        last_vehicle_info_update = datetime.now()
        print("Updated latest_vehicle_info")
        
    elif data.message_type == "realtime_data":
        latest_realtime_data = data.dict()
        last_realtime_data_update = datetime.now()
        print("Updated latest_realtime_data")
    
    return {"status": "success"}

@app.get("/api/vehicle-info")
async def get_vehicle_info():
    print("Received request for vehicle info")
    
    if latest_vehicle_info is None:
        print("Vehicle info not available - waiting for first update")
        raise HTTPException(status_code=404, detail="Vehicle info not available - waiting for first update")
    
    if last_vehicle_info_update is None:
        print("No vehicle info has been posted yet")
        raise HTTPException(status_code=404, detail="No vehicle info has been posted yet")
    
    
    if datetime.now() - last_vehicle_info_update > timedelta(seconds=DATA_TIMEOUT):
        print("Vehicle info is stale - waiting for new update")
        raise HTTPException(status_code=404, detail="Vehicle info is stale - waiting for new update")
    
    print("Returning latest_vehicle_info")
    return latest_vehicle_info

@app.get("/api/realtime-data")
async def get_realtime_data():
    print("Received request for realtime data")
    
    if latest_realtime_data is None:
        print("Realtime data not available - waiting for first update")
        raise HTTPException(status_code=404, detail="Realtime data not available - waiting for first update")
    
    if last_realtime_data_update is None:
        print("No realtime data has been posted yet")
        raise HTTPException(status_code=404, detail="No realtime data has been posted yet")
    
    
    if datetime.now() - last_realtime_data_update > timedelta(seconds=DATA_TIMEOUT):
        print("Realtime data is stale - waiting for new update")
        raise HTTPException(status_code=404, detail="Realtime data is stale - waiting for new update")
        
    print("Returning latest_realtime_data")
    return latest_realtime_data


app.mount("/static", StaticFiles(directory="static"), name="static")

if __name__ == "__main__":
    print("Starting Vehicle Monitoring API server...")
    uvicorn.run(app, host="0.0.0.0", port=8080)
