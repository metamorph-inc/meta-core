import sys, os
import argparse
import json
import math
import cad_library

hpCalcFactor = 0.000191
airDragConst = 0.0033
dragCoefficient = 0.37
maxRpm = 6000
numGears = 6
diffratio = 4.5
frontalArea=20 # square meters?

minRpm=500
shiftingTime=500
timeStep = 1
accFactor = 18.7
totalWeight = 3000
#minTorque=0
#maxTorque=200

tireWidth=215
tireRatio=45
tireIntDia=16
tirecircum =0

gearRatios = [0, 4.449, 2.908, 1.893, 1.446, 1, 0.742]
torqueCurve_rpm = [0, 500, 1000, 1500, 2000, 2500, 3000, 3500, 4000, 4500, 5000, 5500, 6000, 6250, 6500, 6750]
torqueCurve_torque = [0, 800, 1350, 2000, 2450, 2630, 3100, 3500, 4040, 4500, 4750, 4850, 4680, 4620, 4550, 4270]

def update_manifest(tbmanifest, metricvalues):
    if os.path.exists(tbmanifest):
        # read current summary report, which contains the metrics
        with open(tbmanifest, 'r') as file_in:
            result_json = json.load(file_in)

        # update analysis status
        if 'Status' in result_json:
            if os.path.exists('_FAILED.txt'):
                result_json['Status'] = 'FAILED'
            else:
                result_json['Status'] = 'OK'

        if 'Metrics' in result_json:
            for metric in result_json['Metrics']:
                if 'Name' in metric and 'Value' in metric and 'GMEID' in metric:
                    key = metric['Name']
                    if key in metricvalues:
                        # update metric's value to the last value in
                        # time series
                        metric['Value'] = metricvalues[key]
        else:
            # create warning message
            print(('% does not contain Metrics' % tbmanifest))
    else:
        print(('Given result file does not exist: {0}'.format(tbmanifest)))

    # update json file with the new values
    with open(tbmanifest, 'wb') as file_out:
        json.dump(result_json, file_out, indent=4)
    print(('Finished updating %s file.' % tbmanifest))

def calcturnradius(crampangle0, crampangle1, front_left, front_right, rear_left, rear_right):
    cadmetrics = cad_library.CADMetrics
    cadmetrics.read_metrics_file("ComputedValues.xml")

    l = len(cadmetrics.metrics[front_left]-cadmetrics.metrics[rear_left])
    a2 = l/2
    cota0 = math.cos(crampangle0)/math.sin(crampangle0)
    cota1 = math.cos(crampangle1)/math.sin(crampangle1)
    cota = (cota0+cota1)/2

    return math.sqrt(a2*a2+l*l*cota*cota)

def read_torque_curve(filename):
    global torqueCurve_rpm
    global torqueCurve_torque
    try:
        with open(filename, 'r') as file_in:
            torqueCurve_rpm =  []
            torqueCurve_torque = []
            for s in file_in:
                items = s[1:-1].split(';')
                for i in items:
                    j = i.split(',')
                    torqueCurve_rpm.append(float(j[0])*9.54929659643)
                    torqueCurve_torque.append(float(j[1]))
    except:
        pass

def calctopspeed(tire_width, tire_aspect, tire_rimdia, rpm, last_gear, diff):
    #var pi = 3.14159265359;
    width_in = tire_width / 25.4
    tireheight_in = width_in * tire_aspect / 100
    diameter_in = tireheight_in * 2 + tire_rimdia
    radius_in = diameter_in / 2
    circ_in = 2 * math.pi * radius_in
    circ_mile = circ_in * 0.00001578
    TOS6F_rpm = rpm / last_gear
    DOS6F_rpm = TOS6F_rpm / diff
    topSpeed6F = math.floor(DOS6F_rpm * 60 * circ_mile)
    return topSpeed6F

def getTireCircum():
    diaInInches = 0.000787402 * tireWidth * tireRatio + tireIntDia
    return(0.0254 * diaInInches * 3.1416)

def findAirDragAtSpeed(speed):
    return airDragFactor * speed * speed

def getTorque(rpm, gear):
    val = getTorqueCurve(rpm)
    torque = (1.9694 / tirecircum) * diffratio * gearRatios[gear] * val
    return torque

def getSpeedAtRpm(rpm, gear):
    gearRatio = gearRatios[gear]
    tireRpm = rpm / (gearRatio * diffratio)
    return(tireRpm * tirecircum * 60) / 1609.344

def findMinSpeed():
    return getSpeedAtRpm(minRpm, 1)

def getRpmAtSpeed(speed, gear):
    rpmAtTire = (speed * 1609.344 / 60) / tirecircum
    gearRatio = gearRatios[gear]
    return rpmAtTire * gearRatio * diffratio

def findCruisingRpm():
    global cruisingRpm
    cruisingRpm = getRpmAtSpeed(80, numGears)

def findCommonMinMaxSpeeds():
    global commonMinSpeed
    commonMinSpeed = -1
    global commonMaxSpeed
    commonMaxSpeed = -1
    if ((commonMinSpeed == -1) or (minSpeed > commonMinSpeed)):
        commonMinSpeed = minSpeed
    if ((commonMaxSpeed == -1) or (topSpeed < commonMaxSpeed)):
        commonMaxSpeed = topSpeed

def findMaxSpeed():
    rpmLimitedSpeed = getSpeedAtRpm(maxRpm, numGears)
    minSpeed = findMinSpeed() + 1
    for speed in [rpmLimitedSpeed, minSpeed]:
        for gear in [numGears, 1]:
            rpm = getRpmAtSpeed(speed, gear)
            if (rpm > maxRpm):
                break

            torque = getTorque(rpm, gear)
            dragForce = findAirDragAtSpeed(speed)
            if (torque > dragForce):
                return speed

def saveTorque():
    return None

def getTorqueCurve(rpm):
    for i in range(0,len(torqueCurve_rpm)):
        if (torqueCurve_rpm[i]>rpm):
            factor = (rpm-torqueCurve_rpm[i-1])/(torqueCurve_rpm[i]-torqueCurve_rpm[i-1])
            return torqueCurve_torque[i-1]*factor+torqueCurve_torque[i]*(1-factor)
    return torqueCurve_torque[len(torqueCurve_torque)-1]

def findOptGears(minSpeed, maxSpeed):
    maxTimes = 0
    optGear = {}
    bestOptGear = 0
    shiftRpms = 0
    numTicks = shiftingTime / timeStep
    minGearAtMinSpeed = 0
    maxGearAtMinSpeed = 0
    minGearAtMaxSpeed = 0
    maxGearAtMaxSpeed = 0
    for g in range(numGears,0,-1):
        if getRpmAtSpeed(minSpeed, g) < maxRpm:
            minGearAtMinSpeed = g
        if getRpmAtSpeed(maxSpeed, g) < maxRpm:
            minGearAtMaxSpeed = g

    for g in range(1,numGears+1):
        if getRpmAtSpeed(minSpeed, g) > minRpm:
            maxGearAtMinSpeed = g
        if getRpmAtSpeed(maxSpeed, g) > minRpm:
            maxGearAtMaxSpeed = g

    totalTime = -1
    for minGear in range(minGearAtMinSpeed, maxGearAtMinSpeed+1):
        for maxGear in range(minGearAtMaxSpeed, maxGearAtMaxSpeed+1):
            if maxGear != maxGearAtMaxSpeed:
                continue

            if maxGear < minGear:
                continue

            shiftRpms = {}
            time = 0
            speed = minSpeed
            gear = minGear
            counter = 0
            while True:
                torque = 0
                newGear = 0
                for  g in range(gear,maxGear+1):
                    if g > (gear + 1):
                        break
                    rpm = getRpmAtSpeed(speed, g)
                    if rpm < minRpm or rpm > maxRpm:
                        continue

                    temp = getTorque(rpm, g)
                    if torque < temp:
                        torque = temp
                        newGear = g

                if newGear != gear:
                    t = getRpmAtSpeed(speed, gear)
                    if t > maxRpm:
                        shiftRpms[gear] = maxRpm
                    else:
                        shiftRpms[gear] = t

                    for tick in range(1, numTicks+1):
                        optGear[time] = newGear
                        time = time + timeStep
                        force = -findAirDragAtSpeed(speed)
                        speedInc = accFactor * force / totalWeight * timeStep / 1000
                        speed = speed + speedInc

                    gear = newGear

                optGear[time] = gear
                rpm = getRpmAtSpeed(speed, gear)
                force = torque - findAirDragAtSpeed(speed)
                speedInc = accFactor * force / totalWeight * timeStep / 1000
                speed = speed + speedInc
                # print speed
                time = time + timeStep
                if speed >= maxSpeed:
                    break

            tempTotalTime = time
            if (totalTime == -1) or (tempTotalTime < totalTime):
                totalTime = tempTotalTime
                bestOptGear = optGear
                bestShiftRpms = shiftRpms
                bestStartGear = minGear
                optGear = {}
                maxTimes = totalTime

    return maxTimes

def computeacceleration(min, max):
    return findOptGears(min, max)

def calcaccel0_60():
    global topSpeed
    global minSpeed
    global peakTorque
    global airDragFactor
    global tirecircum
    dynoInGear = 4
    tirecircum=getTireCircum()
    saveTorque()
    airDragFactor = airDragConst * dragCoefficient * frontalArea
    topSpeed = findMaxSpeed()
    minSpeed = findMinSpeed()
    findCommonMinMaxSpeeds()
    findCruisingRpm()

    tmpPower = {}
    peakPower = 0
    peakTorque = 0
    for r in range(minRpm, maxRpm+1):
        tmpPower[r] = round(getTorqueCurve(r) * r * hpCalcFactor)
        if getTorqueCurve(r) >= peakTorque:
            peakTorque = getTorqueCurve(r)
            peakTorqueRpm = r

        if tmpPower[r] >= peakPower:
            peakPower = tmpPower[r]
            peakPowerRpm = r

    powers = tmpPower

    return float(computeacceleration(4,60))/1000
    #speedTimeCalculations()
    #calculateGap()

def parseinputs(inputs):
    i = {}
    if inputs is None or len(inputs) == 0:
        return i
    for j in inputs.split(';'):
        if len(j)>0:
            k = j.split('=')
            i[k[0]] = float(k[1])
    return i

def exitwitherror(msg, code, program=''):
    with open('_FAILED.txt', 'a') as f:
        f.write(program + ': ' + msg + '\n')  # python will convert \n to os.linesep
    exit(code)

def processmetric(metric, parameters):

    metricname = metric['Name']

    if metricname=='TurnRadius':
        metricv = calcturnradius(float(parameters['crampangle0']['Value']), float(parameters['crampangle1']['Value']), inputs['front_left'], inputs['front_right'], inputs['rear_left'], inputs['rear_right'])
    elif metricname=='TopSpeed':
        metricv = calctopspeed(float(parameters['tire_width']['Value']), float(parameters['tire_ratio']['Value']), float(parameters['tire_rim_diameter']['Value']), float(parameters['max_rpm']['Value']), float(parameters['last_gear']['Value']), float(parameters['differential_gear_ratio']['Value']))
    elif metricname=='Acc0_60':
        read_torque_curve("torque_curve.txt")
        dragCoefficient = inputs['drag_factor']
        totalWeight = inputs['car_weight']
        diffratio = inputs['diff_ratio']
        frontalArea = inputs['frontal_area']
        numGears = int(inputs['num_gears'])
        gearRatios[1] = inputs['gear_1']
        gearRatios[2] = inputs['gear_2']
        gearRatios[3] = inputs['gear_3']
        gearRatios[4] = inputs['gear_4']
        gearRatios[5] = inputs['gear_5']
        gearRatios[6] = inputs['gear_6']
        metricv = calcaccel0_60()
    else:
        return

    # print metricv
    metricvalues = {metricname:metricv}
    update_manifest('testbench_manifest.json', metricvalues)


def main():
    global dragCoefficient
    global args
    global totalWeight
    global diffratio
    global frontalArea
    global numGears
    try:
        with open('testbench_manifest.json', 'r') as file_in:
            result_json = json.load(file_in)

        parameters = {}

        if 'Parameters' in result_json:
            for param in result_json['Parameters']:
                parameters[param['Name']] = param

        metricvalues = {}

        if 'Metrics' in result_json:
            for metric in result_json['Metrics']:
                processmetric(metric, parameters)

        update_manifest('testbench_manifest.json', metricvalues)
    except Exception as e:
        exitwitherror('Job failed:' + str(e), -1)


if __name__ == '__main__':
    main()